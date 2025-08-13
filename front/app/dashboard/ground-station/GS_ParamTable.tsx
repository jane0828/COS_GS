'use client';
import { useState, useEffect, useRef } from "react";

// 📊 Param Table 컴포넌트 (적응형 레이아웃)
export default function GS_ParamTable({ className = "" }) {
  const [activeTab, setActiveTab] = useState(0);
  const tables = [
    { name: "TX", data: [{ name: "Signal A", value: "87%" }, { name: "Signal B", value: "73%" }, { name: "Signal C", value: "92%" }, { name: "Signal D", value: "65%" }, { name: "Signal E", value: "78%" }] },
    { name: "RX", data: [{ name: "Temp A", value: "36.5°C" }, { name: "Temp B", value: "34.2°C" }, { name: "Temp C", value: "38.1°C" }, { name: "Temp D", value: "35.7°C" }, { name: "Temp E", value: "33.9°C" }] },
    { name: "Conf", data: [{ name: "Voltage A", value: "3.3V" }, { name: "Voltage B", value: "5.0V" }, { name: "Voltage C", value: "12.0V" }, { name: "Voltage D", value: "1.8V" }, { name: "Voltage E", value: "24.0V" }] },
    { name: "Data", data: [{ name: "Packet Rate", value: "1000 ms" }, { name: "Buffer Size", value: "256 KB" }, { name: "Baud Rate", value: "115200 bps" }, { name: "Error Correction", value: "Enabled" }, { name: "Power Mode", value: "Normal" }] },
  ];

  return (
    <div className={`flex flex-col h-full ${className}`}>
      {/* 🏷️ 탭 버튼 */}
      <div className="flex-shrink-0 space-x-2 mb-2">
        {tables.map((table, index) => (
          <button
            key={index}
            className={`px-4 py-2 text-sm font-bold rounded-t-md ${
              activeTab === index ? "bg-blue-500 text-white" : "bg-gray-700 text-gray-300 hover:bg-gray-600"
            }`}
            onClick={() => setActiveTab(index)}
          >
            {table.name}
          </button>
        ))}
      </div>

      {/* 📋 테이블 영역 (적응형 글자 크기) */}
      <div className="flex-grow overflow-y-auto min-h-0 bg-opacity-80 bg-gray-900 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md p-4">
        <table className="w-full border-collapse text-[clamp(0.75rem,1vw,1rem)]">
          <thead>
            <tr className="text-cyan-400 text-sm md:text-base lg:text-lg">
              <th className="border-b border-blue-500 p-2 text-left min-w-0">Name</th>
              <th className="border-b border-blue-500 p-2 text-center min-w-0">Value</th>
              <th className="border-b border-blue-500 p-2 text-center min-w-0">Action</th>
            </tr>
          </thead>
          <tbody>
            {tables[activeTab].data.map((row, index) => (
              <tr key={index} className="text-blue-300">
                <td className="border-b border-blue-500 p-2">{row.name}</td>
                <td className="border-b border-blue-500 p-2 text-center">{row.value}</td>
                <td className="border-b border-blue-500 p-2 text-right">
                  <button className="w-[clamp(4rem,8vw,6rem)] px-2 py-1 bg-pink-500 text-white rounded mr-1 hover:bg-red-500">
                    Set
                  </button>
                  <button className="w-[clamp(4rem,8vw,6rem)] px-2 py-1 bg-green-500 text-white rounded hover:bg-green-400">
                    Update
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
}
