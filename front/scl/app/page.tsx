"use client";
import { useState } from "react";
import { ArrowRightIcon } from "@heroicons/react/24/outline";
import Link from "next/link";
import { inter } from "./ui/fonts";
import LogoHandler from "@/utils/logo";

export default function Page() {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  const handleLogin = async (e: React.FormEvent) => {
    e.preventDefault();

    const response = await fetch("/api/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ email, password }),
    });

    const data = await response.json();
    if (data.success) {
      alert("Login successful!");
    } else {
      alert("Login failed!");
    }
  };

  return (
    <main className="flex min-h-screen flex-col p-6 bg-cover bg-center" style={{ backgroundImage: "url('/main/background.jpg')" }}>
      <LogoHandler className="mt-16 md:mt-16 " />

      {/* 로그인 폼 */}
      <div className="flex grow flex-col items-center justify-center">
        <div className="flex flex-col justify-center gap-6 rounded-lg bg-white bg-opacity-90 px-6 py-10 shadow-lg md:w-1/2 md:px-20">
          <p className={`${inter.className} text-center text-lg antialiased`}>
            <strong>Space Ground Station</strong>
          </p>
          <form onSubmit={handleLogin} className="flex flex-col gap-4">
            <input
              type="email"
              placeholder="Email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              className="w-full rounded-lg border border-gray-300 p-3"
              required
            />
            <input
              type="password"
              placeholder="Password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              className="w-full rounded-lg border border-gray-300 p-3"
              required
            />
            <button
              type="submit"
              className="flex items-center justify-center gap-2 rounded-lg bg-blue-500 px-6 py-3 text-white transition-colors hover:bg-blue-400"
            >
              <span>Log in</span>
              <ArrowRightIcon className="w-5" />
            </button>
          </form>
          <p className="text-center text-sm text-gray-600">
            Don't have an account?{" "}
            <Link href="/signup" className="text-blue-500">
              Sign up
            </Link>
          </p>
        </div>
      </div>
    </main>
  );
}
