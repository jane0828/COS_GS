'use client';
import React, { useState, useEffect } from "react";
import GS_CommandSelector from "./GS_CommandSelector";
import { GS_FTPManager } from "./GS_FTPManager";


export default function Page() {
    return (
      <div className="grid grid-cols-[1.5fr_0.5fr_1.0fr] grid-rows-[1.2fr_1.0fr] gap-4 p-4 w-full h-screen">
        {/* Command (col 1, row 1-2) */}
        <div className="col-span-1 row-span-1 overflow-hidden">
          <GS_CommandSelector
            className="relative p-6 bg-opacity-80 bg-gray-900 
                       border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                       rounded-lg backdrop-blur-md flex flex-col w-full h-full overflow-auto"
          />
        </div>
  
        {/* FTP (col 1, row 3) */}
        <div className="col-start-1 col-span-1 row-start-2 row-span-1 flex">
          <GS_FTPManager
            className="relative p-6 bg-opacity-80 bg-gray-900 
                       border border-green-500 shadow-[0_0_10px_rgba(0,255,0,0.6)] 
                       rounded-lg backdrop-blur-md flex flex-col w-full h-full"
          />
        </div>
  
        {/* Satellite Pass List (col 2, row 2) */}
        {/* <div className="col-start-2 col-span-1 row-start-2 flex">
          <GS_PassList
            className="relative p-6 bg-opacity-80 bg-gray-900 
                       border border-yellow-500 shadow-[0_0_10px_rgba(255,255,0,0.6)] 
                       rounded-lg backdrop-blur-md flex flex-col w-full h-full overflow-auto"
          />
        </div> */}
  
        {/* Beacon Table (col 3, row 1-2) */}
        {/* <div className="col-start-3 col-span-1 row-span-2 flex">
          <GS_BeaconTable
            className="relative p-6 bg-opacity-80 bg-gray-900 
                       border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                       rounded-lg backdrop-blur-md flex flex-col w-full h-full"
          />
        </div> */}
  
        {/* New Component (col 3, row 3) */}
        {/* <div className="col-start-3 col-span-1 row-start-3 flex">
          <GS_NewComponent
            className="relative p-6 bg-opacity-80 bg-gray-900 
                       border border-red-500 shadow-[0_0_10px_rgba(255,0,0,0.6)] 
                       rounded-lg backdrop-blur-md flex flex-col w-full h-full"
          />
        </div> */}
      </div>
    );
  }