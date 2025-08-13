'use client';
import { useState, useEffect } from "react";
import { ChevronDown, ChevronUp, ChevronsUpDown } from "lucide-react";
const predefinedUrls = [
  { name: "Active", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle" },
  { name: "Weather", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=weather&FORMAT=tle" },
  { name: "CubeSat", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=cubesat&FORMAT=tle" },
  { name: "Planet", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=planet&FORMAT=tle" },
  { name: "Starlink", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=starlink&FORMAT=tle" },
  { name: "OneWeb", url: "https://celestrak.org/NORAD/elements/gp.php?GROUP=oneweb&FORMAT=tle" }
];

async function fetchTLEData(url: string) {
  const response = await fetch(url);
  const text = await response.text();

  const lines = text.trim().split("\n");
  const satellites = [];

  for (let i = 0; i < lines.length; i += 3) {
    const name = lines[i].trim();
    const satelliteNumber = lines[i + 1]?.substring(2, 7).trim();
    satellites.push({ name, satelliteNumber, nextAOS: "N/A", nextLOS: "N/A", maxElevation: "N/A" });
  }

  return satellites;
}

export function GS_TLETable({ className = "" }) {
  const [tleUrl, setTleUrl] = useState(predefinedUrls[0].url);
  const [satellites, setSatellites] = useState([]);
  const [searchTerm, setSearchTerm] = useState("");
  const [dropdownOpen, setDropdownOpen] = useState(false);
  const [sortConfig, setSortConfig] = useState(null);

  const handleUpdate = async () => {
    try {
      const data = await fetchTLEData(tleUrl);
      setSatellites(data);
    } catch (error) {
      console.error("Failed to fetch TLE data:", error);
    }
  };

  useEffect(() => {
    handleUpdate();
  }, []);

  const handleSort = (key) => {
    let direction = "asc";
    if (sortConfig?.key === key && sortConfig.direction === "asc") {
      direction = "desc";
    }
    setSortConfig({ key, direction });

    setSatellites((prev) =>
      [...prev].sort((a, b) => {
        const aValue = a[key] || "";
        const bValue = b[key] || "";
        return direction === "asc"
          ? aValue.localeCompare(bValue, undefined, { numeric: true })
          : bValue.localeCompare(aValue, undefined, { numeric: true });
      })
    );
  };

  const getSortIcon = (key) => {
    if (sortConfig?.key === key) {
      return sortConfig.direction === "asc" ? <ChevronUp size={16} /> : <ChevronDown size={16} />;
    }
    return <ChevronsUpDown size={16} />;
  };

  const filteredSatellites = satellites.filter((sat) =>
    sat.name.toLowerCase().includes(searchTerm.toLowerCase())
  );

  return (
    <div className={`font-[Orbitron] ${className}`}>
      {/* URL 입력 및 버튼 영역 */}
      <div className="flex flex-col mb-2 space-y-2">
        <div className="flex items-center space-x-2 relative">
          {/* ▼ 드롭다운 버튼 */}
          <div className="relative">
            <button
              onClick={() => setDropdownOpen(!dropdownOpen)}
              className="w-10 h-10 flex items-center justify-center bg-gray-700 border border-gray-600 rounded-md hover:bg-gray-600 transition"
            >
              <ChevronDown size={20} className="text-white" />
            </button>
            {dropdownOpen && (
              <div className="absolute left-0 top-12 w-48 bg-gray-800 border border-gray-600 rounded-md shadow-lg z-10">
                {predefinedUrls.map((item) => (
                  <button
                    key={item.url}
                    onClick={() => {
                      setTleUrl(item.url);
                      setDropdownOpen(false);
                    }}
                    className="block w-full text-left px-4 py-2 text-white hover:bg-gray-600"
                  >
                    {item.name}
                  </button>
                ))}
              </div>
            )}
          </div>

          {/* URL 입력창 */}
          <input
            type="text"
            value={tleUrl}
            onChange={(e) => setTleUrl(e.target.value)}
            className="flex-grow p-2 bg-gray-800 border border-gray-600 rounded-md text-white focus:outline-none"
            placeholder="Enter TLE URL"
          />

          {/* Update 버튼 */}
          <button
            onClick={handleUpdate}
            className="px-4 py-2 bg-blue-500 text-white rounded-md hover:bg-blue-600 transition"
          >
            Update
          </button>
        </div>

        {/* 검색창 */}
        <input
          type="text"
          value={searchTerm}
          onChange={(e) => setSearchTerm(e.target.value)}
          className="p-2 bg-gray-800 border border-gray-600 rounded-md text-white focus:outline-none"
          placeholder="Search satellite"
        />
      </div>

      {/* 테이블 컨테이너 */}
      <div className="flex-grow overflow-auto border border-gray-600 rounded-md">
        <table className="w-full border-collapse">
          <thead className="sticky top-0 bg-gray-700 text-white">
            <tr>
              {[
                { key: "name", label: "Name" },
                { key: "satelliteNumber", label: "Satellite Number" },
                { key: "nextAOS", label: "Next AOS" },
                { key: "nextLOS", label: "Next LOS" },
                { key: "maxElevation", label: "Max Elevation" },
                { key: "import", label: "Import" }
              ].map(({ key, label }) => (
                <th key={key} className="border border-gray-600 px-4 py-2 cursor-pointer" onClick={() => key !== "import" && handleSort(key)}>
                  <div className="flex items-center justify-center">
                    {label}
                    {key !== "import" && <span className="ml-2">{getSortIcon(key)}</span>}
                  </div>
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            {filteredSatellites.length > 0 ? (
              filteredSatellites.map((sat, index) => (
                <tr key={index} className="hover:bg-gray-600 text-white">
                  <td className="border border-gray-600 px-4 py-2">{sat.name}</td>
                  <td className="border border-gray-600 px-4 py-2 text-center">{sat.satelliteNumber}</td>
                  <td className="border border-gray-600 px-4 py-2 text-center">{sat.nextAOS}</td>
                  <td className="border border-gray-600 px-4 py-2 text-center">{sat.nextLOS}</td>
                  <td className="border border-gray-600 px-4 py-2 text-center">{sat.maxElevation}</td>
                  <td className="border border-gray-600 px-4 py-2 text-center">
                    <button className="px-3 py-1 bg-green-500 text-white rounded-md hover:bg-green-600 transition">Import</button>
                  </td>
                </tr>
              ))
            ) : (
              <tr>
                <td colSpan={6} className="text-center py-4 text-gray-400">No data available</td>
              </tr>
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
}


async function fetchBeaconData() {
  const response = await fetch("/api/beacon");
  if (!response.ok) throw new Error("Failed to fetch beacon data");
  return response.json();
}

export function GS_BeaconTable({ className = "" }) {
  const [beaconData, setBeaconData] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const updateData = async () => {
      try {
        const data = await fetchBeaconData();
        setBeaconData(data);
        setLoading(false);
      } catch (err) {
        setError(err.message);
        setLoading(false);
      }
    };
    updateData();
    const interval = setInterval(updateData, 5000); // 5초마다 업데이트
    return () => clearInterval(interval);
  }, []);

  return (
    <div className={`${className}`}>
      <h2 className="text-lg font-bold text-white mb-2">Beacon Data</h2>
      {loading ? (
        <p className="text-gray-400">Loading...</p>
      ) : error ? (
        <p className="text-red-500">{error}</p>
      ) : (
        <div className="flex-grow overflow-auto border border-gray-600 rounded-md">
          <table className="w-full border-collapse">
            <thead className="sticky top-0 bg-gray-700 text-white">
              <tr>
                <th className="border border-gray-600 px-4 py-2">SatelliteTime</th>
                <th className="border border-gray-600 px-4 py-2">CallSign_1</th>
                <th className="border border-gray-600 px-4 py-2">Battery Status</th>
                <th className="border border-gray-600 px-4 py-2">Temperature</th>
              </tr>
            </thead>
            <tbody>
              {beaconData.length > 0 ? (
                beaconData.map((data, index) => (
                  <tr key={index} className="hover:bg-gray-600 text-white">
                    <td className="border border-gray-600 px-4 py-2 text-center">{data.SatelliteTime}</td>
                    <td className="border border-gray-600 px-4 py-2 text-center">{data.CallSign_1}</td>
                    <td className="border border-gray-600 px-4 py-2 text-center">{data["Battery Status"]}</td>
                    <td className="border border-gray-600 px-4 py-2 text-center">{data.Temperature}</td>
                  </tr>
                ))
              ) : (
                <tr>
                  <td colSpan={4} className="text-center py-4 text-gray-400">
                    No data available
                  </td>
                </tr>
              )}
            </tbody>
          </table>
        </div>
      )}
    </div>
  );
}

export default function Page() {
  return (
    <div className="relative flex flex-col md:flex-row gap-4 p-4">
      <GS_TLETable
        className="relative top-[1rem] left-[1rem] p-6 bg-opacity-80 bg-gray-900 
                   border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                   rounded-lg backdrop-blur-md w-[64rem] h-[30rem] flex flex-col"
      />
      <GS_BeaconTable
        className="relative top-[1rem] right-[1rem] p-6 bg-opacity-80 bg-gray-900 
                   border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                   rounded-lg backdrop-blur-md w-[49rem] h-[28rem] flex flex-col"
      />
    </div>
  );
}
