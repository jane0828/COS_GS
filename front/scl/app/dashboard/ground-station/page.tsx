'use client';
import { orbitron } from "@/app/ui/fonts";
import { useState, useEffect, useRef } from "react";

export function GS_FrequencyDisplayer({ className = "" }) {
  const [uhfTx, setUhfTx] = useState(436.500000);
  const [sBandTx, setSBandTx] = useState(2200.000000);
  const [uhfRx, setUhfRx] = useState(145.800000);
  const [sBandRx, setSBandRx] = useState(2275.000000);

  const [uhfTxRate, setUhfTxRate] = useState(9600); // bps
  const [sBandTxRate, setSBandTxRate] = useState(5000000);
  const [uhfRxRate, setUhfRxRate] = useState(1200);
  const [sBandRxRate, setSBandRxRate] = useState(10000000);

  return (
    <div className={`${className}`}>
      <div className="grid grid-cols-2 gap-6 text-center">
  
        {/* UHF TX */}
        <div className="flex items-center justify-between border-b border-blue-500 pb-4">
          <div className="flex flex-col">
            <span className="text-3xl text-green-400 font-orbitron font-bold">UHF TX</span>
            <span className="text-2xl text-gray-400">{uhfTxRate.toLocaleString()} bps</span>
          </div>
          <div className="flex items-center space-x-1">
            <span className="text-4xl font-bold text-green-300">{uhfTx.toFixed(6)}</span>
            <span className="text-2xl text-green-300">MHz</span>
          </div>
        </div>
  
        {/* S-band TX */}
        <div className="flex items-center justify-between border-b border-blue-500 pb-4">
          <div className="flex flex-col">
            <span className="text-3xl text-blue-400 font-orbitron font-bold">S-band TX</span>
            <span className="text-2xl text-gray-400">{sBandTxRate.toLocaleString()} bps</span>
          </div>
          <div className="flex items-center space-x-1">
            <span className="text-4xl font-bold text-blue-300">{sBandTx.toFixed(6)}</span>
            <span className="text-2xl text-blue-300">MHz</span>
          </div>
        </div>
  
        {/* UHF RX */}
        <div className="flex items-center justify-between">
          <div className="flex flex-col">
            <span className="text-3xl text-green-400 font-orbitron font-bold">UHF RX</span>
            <span className="text-2xl text-gray-400">{uhfRxRate.toLocaleString()} bps</span>
          </div>
          <div className="flex items-center space-x-1">
            <span className="text-4xl font-bold text-green-300">{uhfRx.toFixed(6)}</span>
            <span className="text-2xl text-green-300">MHz</span>
          </div>
        </div>
  
        {/* S-band RX */}
        <div className="flex items-center justify-between">
          <div className="flex flex-col">
            <span className="text-3xl text-blue-400 font-orbitron font-bold">S-band RX</span>
            <span className="text-2xl text-gray-400">{sBandRxRate.toLocaleString()} bps</span>
          </div>
          <div className="flex items-center space-x-1">
            <span className="text-4xl font-bold text-blue-300">{sBandRx.toFixed(6)}</span>
            <span className="text-2xl text-blue-300">MHz</span>
          </div>
        </div>
  
      </div>
    </div>
  );
  
}





// 📊 Param Table 컴포넌트 (버튼 스타일 수정)
function ParamTable({ tableData }: { tableData: { name: string; value: string }[] }) {
  return (
    <div className="p-4 bg-opacity-80 bg-gray-900 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[48rem] h-[28rem] overflow-y-auto">
      <table className="w-full border-collapse table-fixed">
        <thead>
          <tr className="text-cyan-400">
            <th className="border-b border-blue-500 p-2 text-left">Name</th>
            <th className="border-b border-blue-500 p-2 text-center">Value</th>
            <th className="border-b border-blue-500 p-2 text-center">Action</th>
          </tr>
        </thead>
        <tbody>
          {tableData.map((row, index) => (
            <tr key={index} className="text-blue-300">
              <td className="border-b border-blue-500 p-2">{row.name}</td>
              <td className="border-b border-blue-500 p-2 text-center">{row.value}</td>
              <td className="border-b border-blue-500 p-2 text-right">
                <button className="w-20 px-2 py-1 bg-pink-500 text-white rounded mr-1 hover:bg-red-500">
                  Set
                </button>
                <button className="w-20 px-2 py-1 bg-green-500 text-white rounded hover:bg-green-400">
                  Update
                </button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}


// 📌 여러 개의 Param Table을 관리하는 컴포넌트
export function GS_ParamTables({ className = "" }) {
  const [activeTab, setActiveTab] = useState(0);

  // 📋 각 테이블 데이터 (업데이트된 Data 테이블 포함)
  const tables = [
    {
      name: "TX",
      data: [
        { name: "Signal A", value: "87%" },
        { name: "Signal B", value: "73%" },
        { name: "Signal C", value: "92%" },
        { name: "Signal D", value: "65%" },
        { name: "Signal E", value: "78%" },
      ],
    },
    {
      name: "RX",
      data: [
        { name: "Temp A", value: "36.5°C" },
        { name: "Temp B", value: "34.2°C" },
        { name: "Temp C", value: "38.1°C" },
        { name: "Temp D", value: "35.7°C" },
        { name: "Temp E", value: "33.9°C" },
      ],
    },
    {
      name: "Conf",
      data: [
        { name: "Voltage A", value: "3.3V" },
        { name: "Voltage B", value: "5.0V" },
        { name: "Voltage C", value: "12.0V" },
        { name: "Voltage D", value: "1.8V" },
        { name: "Voltage E", value: "24.0V" },
      ],
    },
    {
      name: "Data",
      data: [
        { name: "Packet Rate", value: "1000 ms" },
        { name: "Buffer Size", value: "256 KB" },
        { name: "Baud Rate", value: "115200 bps" },
        { name: "Error Correction", value: "Enabled" },
        { name: "Power Mode", value: "Normal" },
      ],
    },
  ];

  return (
    <div className={`${className}`}>
      {/* 🏷️ 탭 버튼 */}
      <div className="flex space-x-2 mb-2">
        {tables.map((table, index) => (
          <button
            key={index}
            className={`px-4 py-2 text-sm font-bold rounded-t-md ${
              activeTab === index
                ? "bg-blue-500 text-white"
                : "bg-gray-700 text-gray-300 hover:bg-gray-600"
            }`}
            onClick={() => setActiveTab(index)}
          >
            {table.name}
          </button>
        ))}
      </div>

      {/* 📊 선택된 테이블 렌더링 */}
      <ParamTable tableData={tables[activeTab].data} />
    </div>
  );
}
// 📌 Polar Map 컴포넌트
export function GS_PolarMap({ className = "" }) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    // 📌 rem을 px로 변환 (1rem = 16px 기준)
    const remToPx = (rem: number) => rem * 16;
    const size = remToPx(30); // 30rem → px 변환

    // 🔹 Canvas 크기 설정 (2배 해상도로 설정)
    canvas.width = size * 2;
    canvas.height = size * 2;

    // 🔹 컨텍스트 크기 스케일링 (2배 해상도 대비)
    ctx.scale(2, 2);

    const width = canvas.width / 2;
    const height = canvas.height / 2;
    const centerX = width / 2;
    const centerY = height / 2;
    const radius = (Math.max(width, height) / 2 - 10) * 0.8;

    ctx.clearRect(0, 0, width, height);

    // 배경 스타일
    ctx.fillStyle = "rgba(20, 20, 20, 0.9)";
    ctx.fillRect(0, 0, width, height);

    // 외곽 원
    ctx.strokeStyle = "cyan";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
    ctx.stroke();

    // 방위각 선 (위쪽이 0°, 시계방향 90° 간격)
    for (let angle = 0; angle < 360; angle += 90) {
      const correctedAngle = angle - 90; // 위쪽이 0°가 되도록 조정
      const rad = (correctedAngle * Math.PI) / 180;
      const x = centerX + Math.cos(rad) * radius;
      const y = centerY + Math.sin(rad) * radius;

      ctx.beginPath();
      ctx.moveTo(centerX, centerY);
      ctx.lineTo(x, y);
      ctx.strokeStyle = "rgba(0, 255, 255, 0.7)";
      ctx.stroke();

      // 방위각 텍스트 스타일
      ctx.fillStyle = "cyan";
      ctx.font = "bold 16px Arial";
      ctx.lineWidth = 4;

      // 텍스트 위치 조정
      const textX = centerX + Math.cos(rad) * (radius + 15); // 원보다 바깥쪽
      const textY = centerY + Math.sin(rad) * (radius + 15);

      // 텍스트 정렬 자동화 (180°와 270° 포함)
      if (angle === 0) {
        ctx.textAlign = "center";
        ctx.textBaseline = "bottom";
      } else if (angle === 90) {
        ctx.textAlign = "left";
        ctx.textBaseline = "middle";
      } else if (angle === 180) {
        ctx.textAlign = "center";
        ctx.textBaseline = "top"; // 👈 위쪽 정렬
      } else if (angle === 270) {
        ctx.textAlign = "right";
        ctx.textBaseline = "middle";
      }

      // 텍스트 테두리 효과
      ctx.strokeStyle = "black";
      ctx.strokeText(`${angle}°`, textX, textY);
      ctx.fillText(`${angle}°`, textX, textY);
}


    // 고도각 원 (Elevation, 10° 간격)
    for (let el = 0; el <= 90; el += 10) {
      const elRadius = (radius * el) / 90;

      ctx.beginPath();
      ctx.arc(centerX, centerY, elRadius, 0, Math.PI * 2);
      ctx.strokeStyle = "rgba(0, 255, 255, 0.4)";
      ctx.stroke();

      if (el % 20 === 0) {
        const textX = centerX + elRadius;
        const textY = centerY;

        ctx.fillStyle = "cyan";
        ctx.font = "bold 14px Arial";
        ctx.textAlign = "center";
        ctx.textBaseline = "bottom";
        ctx.fillText(`${el}°`, textX, textY);
      }
    }

  }, []);

  return (
    <div className={`${className}`}>
      <canvas ref={canvasRef} className="w-[32rem] h-[30rem]" />
    </div>
  );
}

// 🌍 Rotator Status (Azimuth/Elevation Table)
export function GS_RotatorStatus({ className = "" }) {
  useEffect(() => {
    // Google Fonts에서 Orbitron 로드
    const link = document.createElement("link");
    link.href = "https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&display=swap";
    link.rel = "stylesheet";
    document.head.appendChild(link);
  }, []);

  return (
    <div className={`${className}`}>
      <h2 className="text-cyan-400 font-bold text-lg mb-4 font-orbitron text-center">Rotator Status</h2>
      <table className="w-full border-collapse font-orbitron text-sm">
        <thead>
          <tr className="bg-gray-800 text-cyan-300">
            <th className="border-b border-cyan-500 p-2 text-center w-1/4">ID</th>
            <th className="border-b border-cyan-500 p-2 text-center">Azimuth</th>
            <th className="border-b border-cyan-500 p-2 text-center">Elevation</th>
          </tr>
        </thead>
        <tbody>
          {[
            { id: "Real-Time", az: "30°", el: "10°" },
            { id: "UHF", az: "120°", el: "45°" },
            { id: "S-band", az: "270°", el: "80°" },
          ].map(({ id, az, el }, index) => (
            <tr key={index} className="hover:bg-gray-700 transition">
              <td className="p-3 text-center text-cyan-400">{id}</td>
              <td className="p-3 text-center">{az}</td>
              <td className="p-3 text-center">{el}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export function GS_ImagesDisplay({ className = "", imgName = "", imgTitle = "" }) {
  return (
    <div className={`${className} absolute bottom-4 right-4 flex space-x-4 p-4`}>
      {/* 이미지 컨테이너 */}
      <div className="relative w-full h-full rounded-lg bg-center bg-cover p-2" 
        style={{
          backgroundImage: `url(${imgName})`,
          margin: "0.5rem",
          width: "calc(100% - 1rem)",
          height: "calc(100% - 1rem)",
        }}>

        {/* 제목 (이미지 위쪽, 별도 배경) */}
        <div className="absolute -top-[1rem] left-[1rem] transform bg-gray-900 border border-cyan-400 text-cyan-300 text-lg font-bold px-4 py-2 rounded-md shadow-md tracking-wide font-orbitron">
          {imgTitle}
        </div>
      </div>
    </div>
  );
}

export function GS_ControlPanel({ className = "" }) {
  return (
    <div className={`${className} p-4 bg-gray-900 text-blue-300 border border-blue-500 rounded-lg`}>
      <div className="grid grid-cols-4 gap-4 items-center text-center">
        {/* 1행 */}
        <div className="flex flex-col items-center">
          <span className="text-2xl font-orbitron font-bold text-green-400">UHF Rotator</span>
          <span className="text-lg text-gray-400">9600 bps</span>
        </div>
        <button className="px-4 py-2 bg-pink-500 hover:bg-pink-600 rounded-lg font-bold text-white">
          Reconnect
        </button>
        <button className="px-4 py-2 bg-lime-500 hover:bg-lime-600 rounded-lg font-bold text-black">
          Engage
        </button>
        <button className="px-4 py-2 bg-cyan-500 hover:bg-cyan-600 rounded-lg font-bold text-white">
          Doppler
        </button>
      </div>

      {/* 2행 */}
      <div className="grid grid-cols-4 gap-4 mt-4">
        <input
          type="number"
          placeholder="Azimuth"
          className="flex-1 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center"
        />
        <input
          type="number"
          placeholder="Elevation"
          className="flex-1 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center"
        />
        <button className="px-4 py-2 bg-red-500 hover:bg-red-600 rounded-lg font-bold text-white">
          Set
        </button>
        <button className="px-4 py-2 bg-yellow-500 hover:bg-yellow-600 rounded-lg font-bold text-white">
          Init
        </button>
      </div>

      {/* 3행 */}
      <div className="grid grid-cols-4 gap-4 items-center text-center mt-4">
        <div className="flex flex-col items-center">
          <span className="text-2xl font-orbitron font-bold text-blue-400">S-band</span>
          <span className="text-lg text-gray-400">115200 bps</span>
        </div>
        <button className="px-4 py-2 bg-pink-500 hover:bg-pink-600 rounded-lg font-bold text-white">
          Reconnect
        </button>
        <button className="px-4 py-2 bg-lime-500 hover:bg-lime-600 rounded-lg font-bold text-black">
          Engage
        </button>
        <button className="px-4 py-2 bg-cyan-500 hover:bg-cyan-600 rounded-lg font-bold text-white">
          Doppler
        </button>
      </div>

      {/* 4행 */}
      <div className="grid grid-cols-4 gap-4 mt-4">
        <input
          type="number"
          placeholder="Azimuth"
          className="flex-1 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center"
        />
        <input
          type="number"
          placeholder="Elevation"
          className="flex-1 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center"
        />
        <button className="px-4 py-2 bg-red-500 hover:bg-red-600 rounded-lg font-bold text-white">
          Set
        </button>
        <button className="px-4 py-2 bg-yellow-500 hover:bg-yellow-600 rounded-lg font-bold text-white">
          Init
        </button>
      </div>
    </div>
  );
}


// 🌍 페이지 컴포넌트
export default function Page() {
  // const [isSidebarOpen, setIsSidebarOpen] = useState(getSidebarState());

  return (
    <div>
      <GS_FrequencyDisplayer  className = "absolute top-[1rem] left-[1rem] md:left-20 p-6 bg-opacity-80 bg-gray-900 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[81rem] h-[15rem]"/>
      <GS_ParamTables         className = "absolute top-[17rem] left-[1rem] md:left-20"/> 
      <GS_ControlPanel        className = "absolute top-[49rem] left-[1rem] md:left-20 w-[48rem] h-[17rem]"/>
      <GS_PolarMap            className = "absolute top-[1rem] right-[1rem] bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[32rem] h-[32rem] flex items-center justify-center"/>
      <GS_RotatorStatus       className = "absolute top-[17rem] right-[34rem] transform bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg text-white w-[32rem] h-[16rem]" />
      <GS_ImagesDisplay       className = "absolute top-[34rem] right-[1rem] bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[32rem] h-[32rem] flex " 
                              imgName   = "/img_example/Yagi_Example.jpg"
                              imgTitle  = "UHF"/>
      <GS_ImagesDisplay       className = "absolute top-[34rem] right-[34rem] bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-[32rem] h-[32rem] flex " 
                              imgName   = "/img_example/Parabolic_Example.jpg"
                              imgTitle  = "S-band"/>
    </div>
  );
}
