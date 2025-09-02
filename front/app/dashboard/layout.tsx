'use client';

import { useState, useEffect } from 'react';
import SideNav, { getSidebarState } from '@/app/ui/dashboard/sidenav';
import { getBackgournd_GS } from "@/app/ui/dashboard/background";
import { orbitron } from '../ui/fonts';

export default function Layout({ children }: { children: React.ReactNode }) {
  const [isSidebarOpen, setIsSidebarOpen] = useState(getSidebarState());

  useEffect(() => {
    // 사이드바 상태가 변경될 때마다 업데이트
    const updateSidebarState = () => setIsSidebarOpen(getSidebarState());
    window.addEventListener("sidebarToggle", updateSidebarState);
    return () => window.removeEventListener("sidebarToggle", updateSidebarState);
  }, []);

  return (
    <div className="relative flex h-screen bg-gray-900">
      {/* 🛰️ 사이드바 */}
      <div className={`absolute left-0 top-0 h-full z-50 shadow-lg transition-all duration-300 ${isSidebarOpen ? 'w-64' : 'w-16'} ${orbitron.className}`}>
        <SideNav />
      </div>

      {/* 🌌 콘텐츠 영역 */}
      <div className={`flex-grow min-h-screen transition-all duration-300 ${isSidebarOpen ? 'ml-64' : 'ml-16'} ${getBackgournd_GS()} ${orbitron.className}`}>
        {children}
      </div>
    </div>
  );
}
