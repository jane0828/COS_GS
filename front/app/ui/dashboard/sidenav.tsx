'use client';

import { useState } from 'react';
import { ArrowLeftCircleIcon, Bars3Icon } from '@heroicons/react/24/outline';
import NavLinks from './nav-links';
import LogoHandler from '@/app/ui/logo';

let sidebarState = false; // ✅ 전역 상태 변수

export function getSidebarState() {
  return sidebarState; // ✅ 현재 사이드바 상태 반환
}

export default function Sidebar() {
  const [isOpen, setIsOpen] = useState(false);

  // 사이드바 토글 함수
  const toggleSidebar = () => {
    const newState = !isOpen;
    setIsOpen(newState);
    sidebarState = newState; // ✅ 전역 상태 업데이트
  };

  return (
    <div
      className={`relative h-full flex flex-col justify-between p-4 shadow-lg bg-gray-100 dark:bg-gray-900 transition-all ${
        isOpen ? 'w-64' : 'w-16'
      }`}
    >
      {/* 상단 로고 및 메뉴 */}
      <div>
        {/* 로고 */}
        <div className={`transition-all ${isOpen ? 'block' : 'hidden'}`}>
          <LogoHandler />
        </div>

        {/* 네비게이션 링크 */}
        <nav className="mt-4">
          <NavLinks isOpen={isOpen} />
        </nav>
      </div>

      {/* 하단 사이드바 토글 버튼 */}
      <button onClick={toggleSidebar} className="p-2">
        {isOpen ? (
          <ArrowLeftCircleIcon className="w-8 h-8 text-gray-600 dark:text-white" />
        ) : (
          <Bars3Icon className="w-8 h-8 text-gray-600 dark:text-white" />
        )}
      </button>
    </div>
  );
}
