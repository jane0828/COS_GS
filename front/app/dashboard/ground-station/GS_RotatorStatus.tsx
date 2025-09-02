'use client';
import { useState, useEffect, useRef } from "react";

// 🌍 Rotator Status (Azimuth/Elevation Table)
export default function GS_RotatorStatus({ className = "" }) {

    return (
      <div className={`${className}`}>
        <h2 className="text-cyan-400 font-bold text-lg mb-4  text-center">Rotator Status</h2>
        <table className="w-full border-collapse text-sm">
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
  