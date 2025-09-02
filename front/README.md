250316

# Next.js 프로젝트 설정 및 실행 방법

## 📌 환경 설정

### 1️⃣ Node.js 및 pnpm 설치

이 프로젝트는 [pnpm](https://pnpm.io/)을 사용하여 패키지를 관리합니다. 먼저 Node.js와 pnpm을 설치해야 합니다.

#### ✅ Node.js 설치
[Node.js 공식 사이트](https://nodejs.org/)에서 LTS 버전을 다운로드하여 설치합니다.
또는 `nvm`을 사용하여 설치할 수도 있습니다:

```bash
# nvm이 설치되어 있다면
nvm install --lts
nvm use --lts
```

#### ✅ pnpm 설치

```bash
npm install -g pnpm
```

pnpm이 정상적으로 설치되었는지 확인합니다:
```bash
pnpm -v
```

---

## 🚀 프로젝트 실행 방법

### 1️⃣ 저장소 클론 및 패키지 설치

```bash
git clone <repository_url>
cd <project_directory>
pnpm install
```

### 2️⃣ 환경 변수 파일 설정

`.env.local` 파일을 직접 생성하거나, 기존 `.env.example` 파일을 복사하여 사용합니다:

```bash
cp .env.example .env.local
```

필요한 환경 변수를 `.env.local` 파일에 설정합니다.

---

## 🏗️ 개발 및 빌드

### ✅ 개발 서버 실행
```bash
pnpm dev
```

### ✅ 프로덕션 빌드 및 실행
```bash
pnpm build
pnpm start
```

---

## 📂 `.gitignore` 설정
이 프로젝트에서 Git에 포함되지 않아야 할 파일 및 폴더:
```plaintext
# Node modules
node_modules/

# 빌드 결과물
.next/
out/

# 환경 변수 파일
.env.local

# 로그 및 임시 파일
npm-debug.log*
yarn-debug.log*
yarn-error.log*
pnpm-debug.log*
*.log

# IDE 설정 파일
.vscode/
.idea/

# MacOS 불필요한 파일
.DS_Store
```

---

이제 새로운 환경에서도 Next.js 프로젝트를 쉽게 실행할 수 있습니다! 🚀


