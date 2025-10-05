'use client';
import { useState } from 'react';

export default function Tab() {
  const [activeTab, setActiveTab] = useState(0);

  // Define your tabs and their content
  const tabs = ['Content 1', 'Content 2', 'Content 3'];
  const tabContents = [
    'This is the content for Tab 1',
    'This is the content for Tab 2',
    'This is the content for Tab 3'
  ];

  return (
    <div className="flex flex-col rounded-4xl bg-[#F1F0E4] min-w-140 min-h-120 outline overflow-hidden shadow-md">
      <div className="min-w-full bg-[#37353E] text-center rounded-t-4xl">
        <p className="p-5 text-3xl text-white font-semibold">Tab System</p>
      </div>
      
      <div className="flex flex-row items-stretch bg-[#DDDAD0] text-[#44444E]">
        {tabs.map((tab, index) => (
          <button
            key={index}
            onClick={() => setActiveTab(index)}
            className={`flex-1 py-3 text-lg font-medium transition duration-200
              ${activeTab === index
                ? 'bg-[#37353E] text-white font-bold'
                : 'hover:bg-[#CFCAC0]'
              }`}
          >
            {tab}
          </button>
        ))}
      </div>

      <div className="flex-1 p-6 text-[#44444E] text-lg bg-[#F1F0E4]">
        {tabContents[activeTab]}
      </div>
    </div>
  );
}
