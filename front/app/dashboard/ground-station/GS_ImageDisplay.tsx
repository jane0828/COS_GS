'use client';
import { useState, useEffect, useRef } from "react";

export default function GS_ImagesDisplay({ className = "", imgName = "", imgTitle = "" }) {
    return (
      <div className={`${className}`}>
        {/* 이미지 컨테이너 */}
        <div className="relative w-full h-full rounded-lg bg-center bg-cover p-2" 
          style={{
            backgroundImage: `url(${imgName})`,
            margin: "0.5rem",
            width: "calc(100% - 1rem)",
            height: "calc(100% - 1rem)",
          }}>
  
          {/* 제목 (이미지 위쪽, 별도 배경) */}
          <div className="absolute -top-[1rem] left-[1rem] transform bg-gray-900 border border-cyan-400 text-cyan-300 text-lg font-bold px-4 py-2 rounded-md shadow-md tracking-wide">
            {imgTitle}
          </div>
        </div>
      </div>
    );
  }