'use client';
import { useState, useEffect, useRef } from "react";

export default function GS_ControlPanel({ className = "" }) {
  return (
    <div className={`${className} flex flex-col h-full justify-between overflow-y-auto`}>
      {/* UHF Rotator */}
      <div className="flex flex-col items-center">
        <span className="text-[clamp(1.2rem,2vw,1.5rem)] font-bold text-green-400">
          UHF Rotator
        </span>
      </div>

      {/* 1행 - Azimuth & Elevation */}
      <div className="grid grid-cols-2 gap-4 items-center text-center mt-2">
        <input
          type="number"
          placeholder="Azimuth"
          className="w-full min-w-0 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center text-[clamp(0.8rem,1vw,1rem)]"
        />
        <input
          type="number"
          placeholder="Elevation"
          className="w-full min-w-0 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center text-[clamp(0.8rem,1vw,1rem)]"
        />
      </div>

      {/* 2행 - Set, Init, Engage, Doppler */}
      <div className="grid grid-cols-4 gap-4 mt-2">
        <button className="w-full px-4 py-2 bg-red-500 hover:bg-red-600 rounded-lg font-bold text-white">
          Set
        </button>
        <button className="w-full px-4 py-2 bg-yellow-500 hover:bg-yellow-600 rounded-lg font-bold text-white">
          Init
        </button>
        <button className="w-full px-4 py-2 bg-lime-500 hover:bg-lime-600 rounded-lg font-bold text-black">
          Engage
        </button>
        <button className="w-full px-4 py-2 bg-cyan-500 hover:bg-cyan-600 rounded-lg font-bold text-white">
          Doppler
        </button>
      </div>

      {/* 구분선 */}
      <hr className="border-gray-600 w-full my-4" />

      {/* S-band */}
      <div className="flex flex-col items-center">
        <span className="text-[clamp(1.2rem,2vw,1.5rem)] font-bold text-blue-400">
          S-band
        </span>
      </div>

      {/* 1행 - Azimuth & Elevation */}
      <div className="grid grid-cols-2 gap-4 items-center text-center mt-2">
        <input
          type="number"
          placeholder="Azimuth"
          className="w-full min-w-0 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center text-[clamp(0.8rem,1vw,1rem)]"
        />
        <input
          type="number"
          placeholder="Elevation"
          className="w-full min-w-0 px-2 py-1 bg-gray-700 border border-gray-500 rounded-lg text-center text-[clamp(0.8rem,1vw,1rem)]"
        />
      </div>

      {/* 2행 - Set, Init, Engage, Doppler */}
      <div className="grid grid-cols-4 gap-4 mt-2">
        <button className="w-full px-4 py-2 bg-red-500 hover:bg-red-600 rounded-lg font-bold text-white">
          Set
        </button>
        <button className="w-full px-4 py-2 bg-yellow-500 hover:bg-yellow-600 rounded-lg font-bold text-white">
          Init
        </button>
        <button className="w-full px-4 py-2 bg-lime-500 hover:bg-lime-600 rounded-lg font-bold text-black">
          Engage
        </button>
        <button className="w-full px-4 py-2 bg-cyan-500 hover:bg-cyan-600 rounded-lg font-bold text-white">
          Doppler
        </button>
      </div>
    </div>
  );
}
