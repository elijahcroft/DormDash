import Image from "next/image";
import Navbar from "./components/Navbar";
import Control from "./components/Control";
import Tab from "./components/Tab";
export default function Home() {
  return (
    <div>
      <Navbar/>
      <div className="flex flex-row-reverse bg-[#44444E] p-8">
        <div>hi</div>
         <Control/>
      </div>
     
    </div>
  );
}
