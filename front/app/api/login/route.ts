// app/api/login/route.ts
import { NextResponse } from "next/server";

export async function POST(req: Request) {
  const { email, password } = await req.json();

  // 간단한 mock 인증
  if (email === "test@example.com" && password === "password123") {
    return NextResponse.json({ success: true });
  } else {
    return NextResponse.json({ success: false });
  }
}
