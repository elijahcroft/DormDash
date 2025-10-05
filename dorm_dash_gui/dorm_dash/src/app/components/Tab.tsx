export default function Tab(){
    return (
        <div className="flex flex-col rounded-4xl bg-[#F1F0E4]  min-w-140 min-h-120 outline">
            <div className="min-w-full min-h-30 bg-[#37353E] text-center rounded-t-4xl">
                <p className="p-5 text-3xl">Tab:</p>
            </div>
            <div className="flex flex-row items-stretch bg-[#DDDAD0] min-h-10 text-[#44444E] ">
                <div className="flex min-w-1/3 justify-center">Content 1</div>
                <div className="flex min-w-1/3 justify-center">Content 2</div>
                <div className="flex min-w-1/3 justify-center">Content 3</div>
            </div>
        </div>
    )
}