'use client';
import Image from 'next/image';
import Link from 'next/link';
import { usePathname } from 'next/navigation';

export default function Navbar() {
  const currentPath = usePathname();

  const linkClass = (path: string) => {
    const isActive =
      path === '/'
        ? currentPath === '/'
        : currentPath.startsWith(path);
  
    return `text-xl px-3 py-1 rounded transition duration-200 ${
      isActive
        ? 'bg-zinc-900 text-gray-500 font-bold'
        : 'text-white hover:text-blue-400 underline underline-offset-4'
    }`;
  };
  
  

  return (
    <div className="flex flex-col md:flex-row justify-between items-center px-4 md:px-10 bg-[#37353E] py-4 md:h-20 w-full">
      <div className="flex items-center gap-2 mb-2 md:mb-0">
        <p className="text-10xl md:text-3xl">DormDash</p>
        <Image src="/dormy.png" alt = "Logo" width={60} height={60} />
      </div>
  
      <div className="flex flex-col md:flex-row gap-2 md:gap-x-5">
        <Link href="/dormyinuse" className={linkClass('/dormyinuse')}>
          Queue
        </Link>
      </div>
    </div>
  );
}
