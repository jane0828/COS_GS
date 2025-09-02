import { NextResponse } from "next/server";

export async function POST(req: Request) {
  const { email, password } = await req.json();

  // 간단한 인증 예제 (실제 구현에서는 DB와 연동해야 함)
  if (email === "test@example.com" && password === "password123") {
    return NextResponse.json({ success: true });
  } else {
    return NextResponse.json({ success: false });
  }
}
