import '@/app/ui/global.css';
import FullscreenHandler from '@/utils/fullscreen';
import { LightModeHandler } from '@/utils/lightmode';
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
