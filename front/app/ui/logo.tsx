import Image from "next/image";
import Link from "next/link";

interface LogoHandlerProps {
  className?: string;  // 추가된 부분
}

export default function LogoHandler({ className = "" }: LogoHandlerProps) {
  return (
    <div className={`flex items-center justify-center w-full ${className}`}>
      <Link href={"/dashboard"} className="flex items-center justify-center bg-white bg-opacity-80 dark:bg-gray-700 rounded-lg p-4 shadow-lg">
        <Image
          src="/main/lablogo.png"
          alt="Acme Logo"
          width={200}
          height={80}
          className="h-auto w-auto max-w-[100%] md:max-w-[100%]"
        />
      </Link>
    </div>
  );
}
