import '@/app/ui/global.css';
import FullscreenHandler from '@/app/ui/fullscreen';
import { LightModeHandler } from '@/app/ui/lightmode';
import { inter } from '@/app/ui/fonts'

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body className={`${inter.className} antialiased`}>
        <FullscreenHandler />
          <LightModeHandler>
            {children}
          </LightModeHandler>
      </body>
    </html>
  );
}
