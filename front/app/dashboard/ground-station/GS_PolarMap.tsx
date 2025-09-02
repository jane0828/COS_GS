'use client';
import { useState, useEffect, useRef } from "react";

// 📌 Polar Map 컴포넌트
export default function GS_PolarMap({ className = "" }) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);
  
    useEffect(() => {
      const canvas = canvasRef.current;
      if (!canvas) return;
  
      const ctx = canvas.getContext("2d");
      if (!ctx) return;
  
      const updateCanvasSize = () => {
        // 부모 컨테이너 크기 기준으로 canvas 크기 조정
        const parent = canvas.parentElement;
        if (!parent) return;
  
        const size = Math.min(parent.clientWidth, parent.clientHeight); // 정사각형 크기 조정
  
        canvas.width = size * 2; // 고해상도 대응
        canvas.height = size * 2;
        ctx.scale(2, 2);
  
        drawPolarMap(ctx, size);
      };
  
      // 최초 실행 & resize 감지
      updateCanvasSize();
      window.addEventListener("resize", updateCanvasSize);
      return () => window.removeEventListener("resize", updateCanvasSize);
    }, []);
  
    const drawPolarMap = (ctx: CanvasRenderingContext2D, size: number) => {
      const centerX = size / 2;
      const centerY = size / 2;
      const radius = (size / 2 - 10) * 0.8;
  
      ctx.clearRect(0, 0, size, size);
  
      // 배경
      ctx.fillStyle = "rgba(20, 20, 20, 0.9)";
      ctx.fillRect(0, 0, size, size);
  
      // 외곽 원
      ctx.strokeStyle = "cyan";
      ctx.lineWidth = 2;
      ctx.beginPath();
      ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
      ctx.stroke();
  
      // 방위각 선 및 텍스트
      for (let angle = 0; angle < 360; angle += 90) {
        const rad = ((angle - 90) * Math.PI) / 180;
        const x = centerX + Math.cos(rad) * radius;
        const y = centerY + Math.sin(rad) * radius;
  
        ctx.beginPath();
        ctx.moveTo(centerX, centerY);
        ctx.lineTo(x, y);
        ctx.strokeStyle = "rgba(0, 255, 255, 0.7)";
        ctx.stroke();
  
        // 방위각 텍스트
        ctx.fillStyle = "cyan";
        ctx.font = "bold 16px Arial";
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(`${angle}°`, x, y);
      }
  
      // 고도각 원 (Elevation)
      for (let el = 0; el <= 90; el += 10) {
        const elRadius = (radius * el) / 90;
        ctx.beginPath();
        ctx.arc(centerX, centerY, elRadius, 0, Math.PI * 2);
        ctx.strokeStyle = "rgba(0, 255, 255, 0.4)";
        ctx.stroke();
      }
    };
  
    return (
      <div className={`${className} relative w-full h-full aspect-square flex justify-center items-center`}>
        <canvas ref={canvasRef} className="w-full h-full" />
      </div>
    );
  }
  