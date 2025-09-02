'use client';
import { useState, useEffect, useRef } from "react";

import React from "react";

interface GS_FrequencyDisplayerProps {
  className?: string;
  UHFTx: number;
  sBandTx: number;
  UHFRx: number;
  sBandRx: number;
  UHFTxRate: number;
  sBandTxRate: number;
  UHFRxRate: number;
  sBandRxRate: number;
}

export default function GS_FrequencyDisplayer({
  className = "",
  UHFTx,
  sBandTx,
  UHFRx,
  sBandRx,
  UHFTxRate,
  sBandTxRate,
  UHFRxRate,
  sBandRxRate
}: GS_FrequencyDisplayerProps) {
  return (
    <div className={`${className} w-full mx-auto`}>
      <div className="grid grid-cols-1 sm:grid-cols-2 gap-4 text-center w-full">
        {[ 
          { label: "UHF TX", rate: UHFTxRate, freq: UHFTx, color: "green" },
          { label: "S TX", rate: sBandTxRate, freq: sBandTx, color: "blue" },
          { label: "UHF RX", rate: UHFRxRate, freq: UHFRx, color: "green" },
          { label: "S RX", rate: sBandRxRate, freq: sBandRx, color: "blue" }
        ].map(({ label, rate, freq, color }, index) => (
          <div key={index} className="flex items-center justify-between border-b border-blue-500 pb-3 px-4 w-full">
            <div className="flex flex-col min-w-0 w-1/2 text-left pl-4">
              <span className={`text-${color}-400 font-bold text-[clamp(0.8rem,1.5vw,2rem)]`}> 
                {label} 
              </span>
              <span className="text-gray-400 text-[clamp(0.7rem,1vw,1.2rem)]"> 
                {rate.toLocaleString()} {label.includes("S") ? "Mbps" : "bps"} 
              </span>
            </div>
            <div className="flex items-center space-x-1 w-1/2 justify-end">
              <span className={`font-bold text-${color}-300 text-[clamp(1rem,2vw,2.5rem)]`}> 
                {freq.toFixed(6)} 
              </span>
              <span className={`text-${color}-300 text-[clamp(0.8rem,1.2vw,1.5rem)]`}>MHz</span>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}