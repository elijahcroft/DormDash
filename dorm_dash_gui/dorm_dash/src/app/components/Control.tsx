
export default function Control(){
    return (
        <div className="flex flex-col rounded-4xl bg-[#F1F0E4]  min-w-140 min-h-200 outline">
            <div className="min-w-full min-h-30 bg-[#37353E] text-center rounded-t-4xl">
                <p className="p-5 text-3xl">Control panel:</p>
            </div>
            <div className="flex flex-row items-stretch bg-[#DDDAD0] min-h-10 text-[#44444E] ">
                <div className="flex min-w-1/3 justify-center">tab 1</div>
                <div className="flex min-w-1/3b  justify-center">tab2</div>
                <div className="flex min-w-1/3 justify-center   ">tab3</div>
            </div>
        </div>
    )
}