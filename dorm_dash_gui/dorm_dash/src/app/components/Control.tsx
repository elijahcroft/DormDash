
'use client';

import React, { useState, useEffect } from 'react';

type Tab = 'navigating' | 'addItem' | 'delivering';
type NavigationState = 'input' | 'confirming' | 'navigating';
type DeliveryState = 'input' | 'confirming' | 'waiting';

export default function Control() {
    const [activeTab, setActiveTab] = useState<Tab>('navigating');
    const [dormNumber, setDormNumber] = useState<string>('');
    const [navigationState, setNavigationState] = useState<NavigationState>('input');
    const [progress, setProgress] = useState<number>(0);
    
    // Delivery state
    const [deliveryState, setDeliveryState] = useState<DeliveryState>('input');
    const [destinationDorm, setDestinationDorm] = useState<string>('');
    const [itemDescription, setItemDescription] = useState<string>('');

    const ArrowIcon = () => (
        <svg className="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M14 5l7 7m0 0l-7 7m7-7H3" />
        </svg>
    );

    useEffect(() => {
        let progressInterval: NodeJS.Timeout;
        
        if (navigationState === 'navigating') {
            progressInterval = setInterval(() => {
                setProgress((prev) => {
                    if (prev >= 100) {
                        clearInterval(progressInterval);
                        setNavigationState('input');
                        setProgress(0);
                        return 0;
                    }
                    return prev + 2;
                });
            }, 100);
        }

        return () => {
            if (progressInterval) clearInterval(progressInterval);
        };
    }, [navigationState]);

    const handleDormSubmit = (e: React.FormEvent) => {
        e.preventDefault();
        if (dormNumber.trim()) {
            setNavigationState('confirming');
        }
    };

    const startNavigation = () => {
        setNavigationState('navigating');
    };

    const cancelNavigation = () => {
        setNavigationState('input');
        setDormNumber('');
    };

    const getTabStyle = (tab: Tab) => {
        return `flex flex-1 justify-center items-center ${
            activeTab === tab 
                ? 'font-semibold bg-white text-[#37353E]' 
                : 'hover:bg-white/50 cursor-pointer text-[#37353E]/90'
        }`;
    };

    const handleDeliverySubmit = (e: React.FormEvent) => {
        e.preventDefault();
        if (destinationDorm.trim() && itemDescription.trim()) {
            setDeliveryState('confirming');
        }
    };

    const startDelivery = () => {
        setDeliveryState('waiting');
        setActiveTab('delivering');
    };

    const cancelDelivery = () => {
        setDeliveryState('input');
        setDestinationDorm('');
        setItemDescription('');
    };

    const renderDeliveryContent = () => {
        switch (deliveryState) {
            case 'input':
                return (
                    <form onSubmit={handleDeliverySubmit} className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Add Delivery Request</div>
                        <div className="flex flex-col gap-4 w-72">
                            <div>
                                <label className="block text-xl font-semibold text-[#37353E] mb-2">
                                    Destination Dorm
                                </label>
                                <input
                                    type="number"
                                    value={destinationDorm}
                                    onChange={(e) => setDestinationDorm(e.target.value)}
                                    className="w-full px-6 py-4 bg-[#F1F0E4] border-2 border-[#37353E] rounded-2xl text-2xl text-center focus:outline-none focus:border-[#37353E] focus:ring-2 focus:ring-[#37353E]/20 text-[#37353E] font-semibold"
                                    placeholder="280"
                                    min="1"
                                    max="999"
                                />
                            </div>
                            <div>
                                <label className="block text-xl font-semibold text-[#37353E] mb-2">
                                    Item Description
                                </label>
                                <input
                                    type="text"
                                    value={itemDescription}
                                    onChange={(e) => setItemDescription(e.target.value)}
                                    className="w-full px-6 py-4 bg-[#F1F0E4] border-2 border-[#37353E] rounded-2xl text-xl focus:outline-none focus:border-[#37353E] focus:ring-2 focus:ring-[#37353E]/20 text-[#37353E]"
                                    placeholder="Package description..."
                                />
                            </div>
                        </div>
                        <button
                            type="submit"
                            className="flex items-center gap-2 px-8 py-3 bg-[#37353E] text-white rounded-xl text-xl font-semibold hover:bg-[#37353E]/90 transition-colors shadow-lg hover:shadow-xl transform hover:-translate-y-0.5 transition-all duration-150"
                        >
                            Request Delivery
                            <ArrowIcon />
                        </button>
                    </form>
                );
            case 'confirming':
                return (
                    <div className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Confirm Delivery</div>
                        <div className="flex flex-col gap-4 bg-[#F1F0E4] p-6 rounded-2xl border-2 border-[#37353E]">
                            <div className="text-xl font-semibold text-[#37353E]">
                                Destination: Dorm #{destinationDorm}
                            </div>
                            <div className="text-xl font-semibold text-[#37353E]">
                                Item: {itemDescription}
                            </div>
                        </div>
                        <div className="flex gap-4">
                            <button
                                onClick={startDelivery}
                                className="flex items-center gap-2 px-8 py-3 bg-[#37353E] text-white rounded-xl text-xl hover:bg-[#37353E]/90 transition-all duration-150 shadow-lg hover:shadow-xl transform hover:-translate-y-0.5"
                            >
                                Confirm Request
                                <ArrowIcon />
                            </button>
                            <button
                                onClick={cancelDelivery}
                                className="px-8 py-3 bg-white border-2 border-[#37353E] text-[#37353E] text-xl rounded-xl hover:bg-[#F1F0E4] transition-all duration-150 shadow-lg hover:shadow-xl transform hover:-translate-y-0.5"
                            >
                                Cancel
                            </button>
                        </div>
                    </div>
                );
            case 'waiting':
                return (
                    <div className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Request Submitted</div>
                        <div className="text-xl font-semibold text-[#37353E]">
                            Your delivery request has been added to the queue.
                        </div>
                        <div className="text-xl text-[#37353E]/80">
                            Check the "Delivering" tab for status updates.
                        </div>
                    </div>
                );
        }
    };

    const renderNavigationContent = () => {
        switch (navigationState) {
            case 'input':
                return (
                    <form onSubmit={handleDormSubmit} className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Enter Dorm Number</div>
                        <div className="relative">
                            <input
                                type="number"
                                value={dormNumber}
                                onChange={(e) => setDormNumber(e.target.value)}
                                className="px-6 py-4 bg-[#F1F0E4] border-2 border-[#37353E] rounded-2xl text-3xl w-72 text-center focus:outline-none focus:border-[#37353E] focus:ring-2 focus:ring-[#37353E]/20 text-[#37353E] font-semibold"
                                placeholder="280"
                                min="1"
                                max="999"
                            />
                        </div>
                        <button
                            type="submit"
                            className="flex items-center gap-2 px-8 py-3 bg-[#37353E] text-white rounded-xl text-xl font-semibold hover:bg-[#37353E]/90 transition-colors shadow-lg hover:shadow-xl transform hover:-translate-y-0.5 transition-all duration-150"
                        >
                            Navigate to Dorm
                            <ArrowIcon />
                        </button>
                    </form>
                );
            case 'confirming':
                return (
                    <div className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Confirm Navigation</div>
                        <div className="flex items-center gap-3">
                            <div className="text-2xl font-semibold text-[#37353E] bg-[#F1F0E4] px-8 py-4 rounded-2xl border-2 border-[#37353E]">
                                Dorm #{dormNumber}
                            </div>
                        </div>
                        <div className="flex gap-4">
                            <button
                                onClick={startNavigation}
                                className="flex items-center gap-2 px-8 py-3 bg-[#37353E] text-white rounded-xl text-xl hover:bg-[#37353E]/90 transition-all duration-150 shadow-lg hover:shadow-xl transform hover:-translate-y-0.5"
                            >
                                Start Navigation
                                <ArrowIcon />
                            </button>
                            <button
                                onClick={cancelNavigation}
                                className="px-8 py-3 bg-white border-2 border-[#37353E] text-[#37353E] text-xl rounded-xl hover:bg-[#F1F0E4] transition-all duration-150 shadow-lg hover:shadow-xl transform hover:-translate-y-0.5"
                            >
                                Cancel
                            </button>
                        </div>
                    </div>
                );
            case 'navigating':
                return (
                    <div className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Navigating to Dorm #{dormNumber}</div>
                        <div className="w-96 h-4 bg-[#F1F0E4] rounded-full overflow-hidden border-2 border-[#37353E]">
                            <div 
                                className="h-full bg-[#37353E] transition-all duration-100"
                                style={{ width: `${progress}%` }}
                            />
                        </div>
                        <div className="text-xl font-semibold text-[#37353E]">{progress}% Complete</div>
                    </div>
                );
        }
    };

    const renderContent = () => {
        switch (activeTab) {
            case 'navigating':
                return renderNavigationContent();
            case 'addItem':
                return renderDeliveryContent();
            case 'delivering':
                return (
                    <div className="flex flex-col items-center gap-6 pt-8">
                        <div className="text-3xl font-bold text-[#37353E]">Delivery Status</div>
                        <div className="text-xl font-semibold text-[#37353E]">
                            No active deliveries
                        </div>
                    </div>
                );
        }
    };

    return (
        <div className="flex flex-col rounded-3xl bg-[#F1F0E4] w-[800px] h-[600px] outline">
            <div className="w-full bg-[#37353E] text-left py-4 px-6 rounded-t-3xl">
                <p className="text-3xl text-white">Control Panel:</p>
            </div>
            <div className="flex flex-row items-stretch bg-[#DDDAD0] h-12 text-[#44444E] border-b border-gray-300">
                <div 
                    className={getTabStyle('navigating')}
                    onClick={() => setActiveTab('navigating')}
                >
                    Navigating
                </div>
                <div 
                    className={getTabStyle('addItem')}
                    onClick={() => setActiveTab('addItem')}
                >
                    Add item
                </div>
                <div 
                    className={getTabStyle('delivering')}
                    onClick={() => setActiveTab('delivering')}
                >
                    Delivering
                </div>
            </div>
            <div className="flex-1 bg-white rounded-b-3xl p-4">
                {renderContent()}
            </div>
        </div>
    )
}