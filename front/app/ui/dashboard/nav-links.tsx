'use client';
import {
  WrenchScrewdriverIcon,
  RocketLaunchIcon,
  SignalIcon,
  ChartBarIcon,
} from '@heroicons/react/24/outline';
import Link from 'next/link';
import { usePathname } from 'next/navigation';
import clsx from 'clsx';

// 사이드바 네비게이션 링크 설정
const links = [
  { name: 'Advanced', href: '/dashboard/advanced', icon: WrenchScrewdriverIcon },
  { name: 'Command', href: '/dashboard/command', icon: RocketLaunchIcon },
  { name: 'Ground Station', href: '/dashboard/ground-station', icon: SignalIcon },
  { name: 'Telemetry', href: '/dashboard/telemetry', icon: ChartBarIcon },
];

export default function NavLinks({ isOpen }: { isOpen: boolean }) {
  const pathname = usePathname();
  return (
    <>
      {links.map((link) => {
        const LinkIcon = link.icon;
        return (
          <Link
            key={link.name}
            href={link.href}
            className={clsx(
              'flex items-center gap-2 p-3 text-sm font-medium rounded-md transition-all',
              'dark:bg-gray-800 dark:text-white dark:hover:bg-gray-700 dark:hover:text-sky-400',
              pathname === link.href
                ? 'bg-sky-100 text-blue-600 dark:bg-gray-700 dark:text-sky-400'
                : 'bg-gray-50 hover:bg-sky-100 dark:bg-gray-900 dark:hover:bg-gray-700',
              isOpen ? 'justify-start w-full' : 'justify-center w-12'
            )}
          >
            <LinkIcon className="w-6" />
            {isOpen && <p className="hidden md:block">{link.name}</p>}
          </Link>
        );
      })}
    </>
  );
}
