'use client';
import { useEffect } from "react";

export default function FullscreenHandler() {
  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (event.key === "F4") {
        event.preventDefault(); // 기본 새로고침 방지
        if (!document.fullscreenElement) {
          document.documentElement.requestFullscreen().catch(console.error);
        }
      } else if (event.key === "Escape") {
        if (document.fullscreenElement) {
          document.exitFullscreen().catch(console.error);
        }
      }
    };

    document.addEventListener("keydown", handleKeyDown);
    return () => {
      document.removeEventListener("keydown", handleKeyDown);
    };
  }, []);

  return null;
}
