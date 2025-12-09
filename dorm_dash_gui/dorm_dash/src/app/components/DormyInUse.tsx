export default function DormyInUse() {
  return (
    <div className="flex flex-col rounded-3xl bg-[#F1F0E4] min-w-[35rem] min-h-[25rem] shadow-lg overflow-hidden">
      
      {/* Header */}
      <div className="w-full bg-[#37353E] text-left py-4 px-6">
        <p className="text-2xl font-bold text-white">Queue</p>
      </div>

      {/* Main content */}
      <div className="flex flex-col justify-center items-center flex-grow text-[#44444E] bg-[#DDDAD0]">
        <p className="text-3xl font-semibold mb-2">Dormy is currently in use 😞</p>
        <p className="text-lg">Please wait until the system is available again.</p>
      </div>
    </div>
  );
}
