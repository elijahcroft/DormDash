import Image from "next/image";
import Navbar from "./components/Navbar";
import Control from "./components/Control";
import Tab from "./components/Tab";
export default function Home() {
  return (
    <div className="min-h-screen">
      <Navbar/>
      <div className="flex justify-center items-start bg-[#44444E] p-8 min-h-[calc(100vh-80px)]">
        <Control/>
      </div>
    </div>
  );
}
