#include "../inc/ros/bitstream.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>
#include <ctime>


using std::string;

Bitstream::Bitstream() : opts_{} {}
Bitstream::Bitstream(const Options& opt) : opts_(opt) {}
Bitstream::~Bitstream() { if (ofs_.is_open()) abort(); }

void Bitstream::set_image_debug_mode(bool on) { opts_.image_debug_mode = on; }
void Bitstream::set_options(const Options& o) { opts_ = o; }

static bool in_jpeg = false;
static std::vector<uint8_t> cur_jpeg;
static uint8_t  last_byte_prev = 0x00;

static std::vector<uint8_t> g_acc;   // 누적 버퍼 (포맷 불문)
static size_t g_frag_count = 0;


bool Bitstream::save_final() {
    if (stream_acc_.empty()) {
        std::fprintf(stderr, "[bitstream] FINAL save skipped: empty\n");
        return false;
    }
    if (saved_final_) {
        std::fprintf(stderr, "[bitstream] FINAL already saved, skipping\n");
        return true;
    }
    saved_final_ = true;
    const std::string base = opts_.base_dir.empty() ? "./data/image" : opts_.base_dir;
    if (!ensure_dir(base)) { std::perror("[bitstream] ensure_dir(final)"); return false; }

    std::string stem = opts_.prefix.empty() ? "image-" : opts_.prefix;
    if (opts_.add_datetime) stem += now_timestamp();
    const std::string filename = stem + (opts_.ext.empty()? ".bin" : opts_.ext);
    const std::string path = join_path(base, filename);

    FILE* fp = std::fopen(path.c_str(), "wb");
    if (!fp) { std::perror("[bitstream] fopen(final)"); return false; }

    size_t wr = std::fwrite(stream_acc_.data(), 1, stream_acc_.size(), fp);
    std::fclose(fp);
    if (wr != stream_acc_.size()) {
        std::fprintf(stderr, "[bitstream] FINAL write short: %zu/%zu\n", wr, stream_acc_.size());
        return false;
    }
    std::fprintf(stderr, "[bitstream] FINAL saved: %s (bytes=%zu)\n", path.c_str(), stream_acc_.size());
    return true;
}

void Bitstream::clear() {
    stream_acc_.clear();
    frag_count_ = 0;
    saved_final_ = false;
}

size_t Bitstream::total_bytes() const {
    return stream_acc_.size();
}

static bool ensure_dir(const char* path){
    struct stat st{};
    if (stat(path, &st) == 0) return S_ISDIR(st.st_mode);
    return ::mkdir(path, 0755) == 0;
}



void Bitstream::push_ccsds_fragment(uint16_t /*apid*/, uint16_t seq,
                                    const uint8_t* data, size_t len)
{
    if (!data || len == 0) return;
    ++frag_count_;

    size_t before = stream_acc_.size();
    stream_acc_.insert(stream_acc_.end(), data, data + len);
    size_t after = stream_acc_.size();

    // std::fprintf(stderr,
    //     "[bitstream] append frag#%zu seq=0x%04X len=%zu -> total=%zu (+%zu)\n",
    //     frag_count_, seq, len, after, after - before);

    const size_t show = (len < 32 ? len : 32);
    // if (show > 0) {
    //     std::fprintf(stderr, "[bitstream] frag head bytes:\n");
    //     for (size_t i = 0; i < show; ++i)
    //         std::fprintf(stderr, "%02X%s", data[i], ((i+1)%16==0) ? "\n" : " ");
    //     std::fprintf(stderr, "\n");
    //     if (len > 32) {
    //         std::fprintf(stderr, "[bitstream] frag tail bytes:\n");
    //         for (size_t i = len - show; i < len; ++i)
    //             std::fprintf(stderr, "%02X%s", data[i], ((i+1)%16==0) ? "\n" : " ");
    //         std::fprintf(stderr, "\n");
    //     }
    // }

    if (opts_.image_debug_mode) {
        if (!ensure_dir(opts_.base_dir.empty() ? "./data/image" : opts_.base_dir)) {
            std::perror("[bitstream] ensure_dir");
            return;
        }
        char path[512];
        std::snprintf(path, sizeof(path), "%s/%sfrag%06zu_total%zu%s",
                      (opts_.base_dir.empty()? "./data/image" : opts_.base_dir.c_str()),
                      (opts_.prefix.empty()?   "image-"        : opts_.prefix.c_str()),
                      frag_count_, stream_acc_.size(),
                      (opts_.ext.empty()?      ".bin"          : opts_.ext.c_str()));
        if (FILE* fp = std::fopen(path, "wb")) {
            std::fwrite(stream_acc_.data(), 1, stream_acc_.size(), fp);
            std::fclose(fp);
            std::fprintf(stderr, "[bitstream] snapshot saved: %s\n", path);
        } else {
            std::perror("[bitstream] fopen(snapshot)");
        }
    }
}


bool Bitstream::ensure_dir(const std::string& p) {
    std::error_code ec;
    if (p.empty()) return false;
    if (std::filesystem::exists(p, ec)) return true;
    return std::filesystem::create_directories(p, ec);
}

std::string Bitstream::join_path(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    std::filesystem::path pa(a);
    pa /= b;
    return pa.string();
}

std::string Bitstream::now_timestamp() {
    using clock = std::chrono::system_clock;
    auto t = clock::to_time_t(clock::now());
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

bool Bitstream::start_new() {
    string stem = opts_.prefix;
    if (opts_.add_datetime) stem += now_timestamp();
    return start_new_with_name(stem);
}

bool Bitstream::start_new_with_name(const std::string& filename_no_ext) {
    if (ofs_.is_open()) abort();

    if (!ensure_dir(opts_.base_dir)) {
        if (opts_.image_debug_mode)
            std::cerr << "[Bitstream] 디렉터리 생성 실패: " << opts_.base_dir << "\n";
        return false;
    }

    final_path_ = join_path(opts_.base_dir, filename_no_ext + opts_.ext);
    tmp_path_   = final_path_ + ".part";

    ofs_.open(tmp_path_, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!ofs_) {
        if (opts_.image_debug_mode)
            std::cerr << "[Bitstream] 파일 오픈 실패: " << tmp_path_ << "\n";
        tmp_path_.clear();
        final_path_.clear();
        return false;
    }

    if (opts_.image_debug_mode)
        std::cerr << "[Bitstream] 시작: " << tmp_path_ << "\n";

    return true;
}

bool Bitstream::append(const void* data, size_t len) {
    if (!ofs_.is_open() || data == nullptr || len == 0) return false;
    ofs_.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(len));
    if (!ofs_) {
        if (opts_.image_debug_mode)
            std::cerr << "[Bitstream] write 실패\n";
        return false;
    }
    return true;
}

bool Bitstream::flush() {
    if (!ofs_.is_open()) return false;
    ofs_.flush();
    return static_cast<bool>(ofs_);
}

bool Bitstream::finish() {
    if (!ofs_.is_open()) return false;
    ofs_.flush();
    ofs_.close();

    std::error_code ec;
    std::filesystem::rename(tmp_path_, final_path_, ec);
    if (ec) {
        // 같은 파일시스템이 아니거나 기존 파일이 있으면 실패할 수 있으니 fallback
        std::filesystem::remove(final_path_, ec); // 무시
        std::filesystem::copy_file(tmp_path_, final_path_,
                                   std::filesystem::copy_options::overwrite_existing, ec);
        std::filesystem::remove(tmp_path_, ec);
    }

    if (opts_.image_debug_mode)
        std::cerr << "[Bitstream] 완료: " << final_path_ << "\n";

    tmp_path_.clear();
    return true;
}

bool Bitstream::abort() {
    if (!ofs_.is_open()) return false;
    ofs_.close();
    std::error_code ec;
    std::filesystem::remove(tmp_path_, ec);
    if (opts_.image_debug_mode)
        std::cerr << "[Bitstream] 중단/삭제: " << tmp_path_ << "\n";
    tmp_path_.clear();
    final_path_.clear();
    return true;
}

bool Bitstream::is_open() const { return ofs_.is_open(); }
std::string Bitstream::current_path() const { return is_open() ? tmp_path_ : string(); }
std::string Bitstream::last_final_path() const { return final_path_; }
