// inc/ros/image_extract.h
#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>

class Bitstream;

struct ImageAssemblyConfig {
  uint16_t image_apid;
  size_t   sec_hdr_len;

  size_t   fixed_payload_bytes = 128;
  bool     has_fixed_payload   = true;
};

struct ImageAssemblyReport {
  // [SESSION]
  std::string file_id;
  bool expedite_present = false;

  // 추가: 소스 raw 경로 / 최종 이미지 경로
  std::string source_raw_path;     // 새로 추가
  std::string output_image_path;   // 새로 추가

  // [RECEIVED]
  size_t packets_ok = 0;
  size_t total_image_bytes = 0;    // 새로 추가
  std::vector<uint16_t> seq_seen;
  std::vector<size_t>   rx_bytes;
  size_t expected_payload_len_per_pkt = 0;

  // [SEQUENCE]
  bool has_first = false, has_cont = false, has_last = false;
  bool continuity_ok = false;
  std::vector<uint16_t> missing_seq;
  bool     first_seq_valid = false;
  uint16_t first_seq = 0;
  bool     last_seq_valid  = false;
  uint16_t last_seq  = 0;

  // [JPEG]
  bool soi_ok = false;
  bool eoi_ok = false;

  // [REREQUEST]
  std::vector<std::pair<size_t,size_t>> missing_byte_ranges;
};

// raw_src_path는 선택 인자(없으면 "")
bool assemble_image_from_acc(const std::vector<uint8_t>& acc,
                             const ImageAssemblyConfig& cfg,
                             Bitstream& out_bitstream,
                             ImageAssemblyReport* out_report,
                             const std::string& raw_src_path = {});
