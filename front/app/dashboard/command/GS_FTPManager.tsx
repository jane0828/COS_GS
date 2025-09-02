'use client';
import React, { useState, useEffect } from "react";

export function GS_FTPManager({ className = "" }) {
    const [localURL, setLocalURL] = useState("");
    const [remoteURL, setRemoteURL] = useState("");
    const [progress, setProgress] = useState(0);
    const [status, setStatus] = useState("");
  
    // FTP 파일 업로드 함수
    const handleUpload = async () => {
      if (!localURL || !remoteURL) {
        setStatus("Local URL과 Remote URL을 입력하세요.");
        return;
      }
  
      setStatus("Uploading...");
      setProgress(10);
  
      try {
        const response = await fetch("/api/ftp/upload", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ localURL, remoteURL }),
        });
  
        const result = await response.json();
        if (result.success) {
          setProgress(100);
          setStatus("Upload Complete!");
        } else {
          setStatus("Upload Failed.");
        }
      } catch (error) {
        setStatus("Error during upload.");
      }
    };
  
    // FTP 파일 다운로드 함수
    const handleDownload = async () => {
      if (!remoteURL || !localURL) {
        setStatus("Remote URL과 Local URL을 입력하세요.");
        return;
      }
  
      setStatus("Downloading...");
      setProgress(10);
  
      try {
        const response = await fetch("/api/ftp/download", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ remoteURL, localURL }),
        });
  
        const result = await response.json();
        if (result.success) {
          setProgress(100);
          setStatus("Download Complete!");
        } else {
          setStatus("Download Failed.");
        }
      } catch (error) {
        setStatus("Error during download.");
      }
    };
  
    return (
      <div className={`p-4 bg-gray-800 text-white rounded-md shadow-md ${className}`}>
  
        {/* Local URL 입력 */}
        <div className="flex items-center mb-2">
          <label className="w-1/5 text-left pr-2">Local URL</label>
          <input
            type="text"
            value={localURL}
            onChange={(e) => setLocalURL(e.target.value)}
            className="w-full p-2 rounded-md bg-gray-700 text-white border border-gray-500"
            placeholder="/home/user/file.txt"
          />
        </div>
  
        {/* Remote URL 입력 */}
        <div className="flex items-center mb-2">
          <label className="w-1/5 text-left pr-2">Remote URL</label>
          <input
            type="text"
            value={remoteURL}
            onChange={(e) => setRemoteURL(e.target.value)}
            className="w-full p-2 rounded-md bg-gray-700 text-white border border-gray-500"
            placeholder="ftp://server.com/file.txt"
          />
        </div>
  
        {/* 업로드 / 다운로드 버튼 */}
        <div className="flex gap-2 mb-3">
          <button onClick={handleUpload} className="flex-1 p-2 bg-blue-600 hover:bg-blue-700 rounded-md">
            Upload
          </button>
          <button onClick={handleDownload} className="flex-1 p-2 bg-green-600 hover:bg-green-700 rounded-md">
            Download
          </button>
        </div>
  
        {/* ProgressBar */}
        <div className="mt-3">
          <p className="text-sm font-bold mb-1">ProgressBar</p>
          <div className="h-4 w-full bg-gray-600 rounded-full overflow-hidden relative">
            <div
              className="h-full bg-blue-500 transition-all"
              style={{ width: `${progress}%` }}
            ></div>
            <span className="absolute w-full text-center text-xs font-bold text-white">
              {progress}%
            </span>
          </div>
        </div>
  
        {/* 상태 메시지 */}
        <p className="text-sm text-gray-300 mt-2">{status}</p>
      </div>
    );
  }