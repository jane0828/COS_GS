'use client';
import { useState, useEffect, useRef } from "react";
import { WS_ParameterReceive, WS_ParameterRequest } from "@/app/api/ws/io";
import { hs2uint32 } from "@/app/api/ws/util";

import GS_ControlPanel from "./GS_ControlPanel";
import GS_FrequencyDisplayer from "./GS_FrequencyDisplayer";
import GS_ImagesDisplay from "./GS_ImageDisplay";
import GS_ParamTable from "./GS_ParamTable";
import GS_PolarMap from "./GS_PolarMap";
import GS_RotatorStatus from "./GS_RotatorStatus";


// 🌍 페이지 컴포넌트
export default function Page() {

  const [UHFTx, setUHFTx] = useState(0.0);
  const [sBandTx, setSBandTx] = useState(0.0);
  const [UHFRx, setUHFRx] = useState(0.0);
  const [sBandRx, setSbandRx] = useState(0.0);

  const [UHFTxRate, setUHFTxRate] = useState(0);
  const [sBandTxRate, setSBandTxRate] = useState(0);
  const [UHFRxRate, setUHFRxRate] = useState(0);
  const [sBandRxRate, setSBandRxRate] = useState(0);
  
  useEffect(() => {
    const socket = new WebSocket("ws://hyvrid.yonsei.ac.kr:4443"); // 실제 서버 주소로 수정

    socket.onopen = () => {
      const requests = [
        { table_id: 1, name: "freq" },
        { table_id: 1, name: "baud" },
        { table_id: 5, name: "freq" },
        { table_id: 5, name: "baud" },
      ];

      const message = WS_ParameterRequest("get_param_table_json");
      requests.forEach((params) => {
        const message = WS_ParameterRequest("get_param_json", params);
        console.log("Sending:", message);
        socket.send(message);
      });
    };


    socket.onmessage = (event) => {
      const parsed = WS_ParameterReceive(event.data);

      if (parsed && parsed.reqtype && parsed.data) {
        switch (parsed.reqtype) {
          case "gs100_get_param":
            if(parsed.data.table_id === 1) {
              switch(parsed.data.name) {
                case "freq":
                  setUHFRx(hs2uint32(parsed.data.data));
                  break;
                case "baud":
                  setUHFRxRate(hs2uint32(parsed.data.data));
                  break;
              }
            }
            else if(parsed.data.table_id === 5) {
              switch(parsed.data.name) {
                case "freq":
                  setUHFTx(hs2uint32(parsed.data.data));
                  break;
                case "baud":
                  setUHFTxRate(hs2uint32(parsed.data.data));
                  break;
              }
            }

            break;
          case "gs100_get_param_table":
            break;
          case "report":
            break;
          
        }
      }
    };

    return () => {
      socket.close();
    };
  });

  return (
    <div className="grid grid-cols-[1.2fr_1.2fr_0.6fr_0.6fr] grid-rows-[0.6fr_0.8fr_0.2fr_1.0fr] gap-4 p-4 w-full h-screen">
      
      {/* 1행 - 주파수 디스플레이 */}
      <div className="col-span-2 row-span-1">
        <GS_FrequencyDisplayer
          className="p-6 bg-opacity-80 bg-gray-900 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-full h-full"
          UHFTx={UHFTx}
          sBandTx={sBandTx}
          UHFRx={UHFRx}
          sBandRx={sBandRx}
          UHFTxRate={UHFTxRate}
          sBandTxRate={sBandTxRate}
          UHFRxRate={UHFRxRate}
          sBandRxRate={sBandRxRate}
          />
      </div>

      {/* 2행 - 폴라 맵 (비율 유지) */}
      <div className="col-start-3 col-span-2 row-span-3 flex items-center justify-center">
        <GS_PolarMap
          className="bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-full h-full"
          />
      </div>

      {/* 1행 - 좌측 파라미터 테이블 */}
      <div className="col-start-1 col-span-1 row-start-2 row-span-2 flex">
        <GS_ParamTable
          className="w-full h-full"
          />
      </div>

      {/* 3행 - 중간 로테이터 상태 */}
      <div className="col-start-3 col-span-2 row-start-4 flex items-center">
        <GS_RotatorStatus
          className="bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg text-white w-full h-full"
          />
      </div>

      {/* 4행 - 컨트롤 패널 */}
      <div className="col-start-2 col-span-1 row-start-2 row-span-2 flex items-center">
        <GS_ControlPanel
          className=" p-4 bg-gray-900 text-blue-300 border border-blue-500 rounded-lg w-full h-full"
          />
      </div>

      {/* 4행 - 이미지 (UHF) */}
      <div className="col-start-1 col-span-1 row-start-4 row-span-1">
        <GS_ImagesDisplay
          className="bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-full h-full flex items-center justify-center" imgName="/img_example/Yagi_Example.jpg" imgTitle="UHF"
          />
      </div>

      {/* 4행 - 이미지 (S-band) */}
      <div className="col-start-2 col-span-1 row-start-4 row-span-1">
        <GS_ImagesDisplay
          className="bg-gray-900 p-4 border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] rounded-lg backdrop-blur-md w-full h-full flex items-center justify-center" imgName="/img_example/Parabolic_Example.jpg" imgTitle="S-band"
          />
      </div>
    </div>
  );
}

