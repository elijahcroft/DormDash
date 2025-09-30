
export default function Control(){
    return (
        <div className="flex flex-col rounded-4xl bg-[#F1F0E4]  min-w-140 min-h-120 outline">
            <div className="min-w-full min-h-30 bg-[#37353E] text-left py-4 px-6 rounded-t-4xl">
                <p className="p-5 text-3xl text-white">Control Panel:</p>
            </div>
            <div className="flex flex-row items-stretch bg-[#DDDAD0] min-h-10 text-[#44444E] ">
                <div className="flex flex-1 justify-center items-center">Navigating</div>
                <div className="flex flex-1  justify-center items-center">Add item</div>
                <div className="flex flex-1 justify-center items-center">Delivering</div>
            </div>
        </div>
    )
}