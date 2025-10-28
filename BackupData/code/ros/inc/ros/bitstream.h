#pragma once
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

class Bitstream {
public:
    struct Options {
        // 미래 대비: 디버그 모드(로그/파일명 표시 강화)
        bool image_debug_mode = false;

        // 저장 경로/파일명 구성
        std::string base_dir = "./data/image";
        std::string prefix   = "image-";
        std::string ext      = ".jpg";   // 요구사항 유지: 이름은 .jpg, 내용은 raw bitstream
        bool add_datetime    = true;     // 파일명에 타임스탬프 붙이기
    };

    Bitstream();
    explicit Bitstream(const Options& opt);
    ~Bitstream();

    // 새 비트스트림 파일 시작 (.part로 생성 후 finish()에서 원자적 rename)
    bool start_new();  // prefix + timestamp + ext
    bool start_new_with_name(const std::string& filename_no_ext); // 직접 이름 지정

    // 데이터 추가(그냥 raw 바이트 그대로)
    bool append(const void* data, size_t len);

    // 명시적 flush/완료/중단
    bool flush();
    bool finish(); // .part -> 최종 파일명으로 rename
    bool abort();  // 임시파일 삭제

    bool is_open() const;
    std::string current_path() const;    // 열려있으면 .part 경로, 아니면 빈 문자열
    std::string last_final_path() const; // 마지막 완료된 최종 경로

    // 옵션/디버그
    void set_image_debug_mode(bool on);
    void set_options(const Options& o);

    void push_ccsds_fragment(uint16_t apid, uint16_t seq, const uint8_t* data, size_t len);

private:
    Options opts_;
    std::ofstream ofs_;
    std::string tmp_path_;
    std::string final_path_;

    std::vector<uint8_t> stream_acc_;

    static std::string now_timestamp();           // "YYYYmmdd_HHMMSS"
    static bool ensure_dir(const std::string& p); // 디렉터리 보장
    static std::string join_path(const std::string& a, const std::string& b);
};
