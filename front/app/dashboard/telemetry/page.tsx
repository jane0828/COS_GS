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
    <div className={`${className}`}>
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
  const [beaconData, setBeaconData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [beaconNo, setBeaconNo] = useState(""); // Beacon No. 저장

  useEffect(() => {
    const updateData = async () => {
      try {
        const response = await fetch("/api/beacon");
        if (!response.ok) throw new Error("Failed to fetch beacon data");
        const data = await response.json();

        // CallSign 변환
        const callSignKeys = ["CallSign_1", "CallSign_2", "CallSign_3", "CallSign_4", "CallSign_5", "CallSign_6"];
        const callSignStr = callSignKeys.map((key) => String.fromCharCode(data[key])).join("").trim();

        // "No" 필드를 Beacon No. 값으로 저장하고, 데이터에서 제거
        const beaconNumber = data["No"];
        delete data["No"];

        // 새로운 객체 생성
        const processedData = {
          CallSign: callSignStr, // 변환된 콜사인
          ...data,
        };

        // 기존 CallSign_1 ~ CallSign_6 제거
        callSignKeys.forEach((key) => delete processedData[key]);

        setBeaconNo(beaconNumber);
        setBeaconData(processedData);
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
      {loading ? (
        <p className="text-gray-400">Loading...</p>
      ) : error ? (
        <p className="text-red-500">{error}</p>
      ) : beaconData ? (
        <div className="overflow-auto border border-gray-600 rounded-md">
          <table className="w-full border-collapse">
            {/* 테이블 헤더 */}
            <thead>
              <tr>
                <th colSpan="6" className="border border-gray-600 px-4 py-2 text-center bg-gray-700 text-white text-xl font-bold">
                  Beacon No.
                  <span className="mx-2 text-yellow-400 text-2xl font-extrabold">{beaconNo}</span>
                </th>
              </tr>
            </thead>
            <tbody>
              {(() => {
                const entries = Object.entries(beaconData);
                const rows = [];

                // 2열씩 한 행에 배치 (필드명, 값 반복)
                for (let i = 0; i < entries.length; i += 3) {
                  rows.push(
                    <tr key={i} className="hover:bg-gray-600 text-white">
                      {Array.from({ length: 3 }).map((_, j) => {
                        const index = i + j;
                        if (index < entries.length) {
                          const [key, value] = entries[index];
                          return (
                            <>
                              <td className="border border-gray-600 px-4 py-2 text-left text-gray-300 w-2/3">{key}</td> {/* 홀수열 - 일반 글씨, 넓은 칸 */}
                              <td className="border border-gray-600 px-4 py-2 text-center text-blue-400 font-bold w-1/3">{value}</td> {/* 짝수열 - 볼드, 파란색 */}
                            </>
                          );
                        } else {
                          // 빈 셀 추가 (열 맞추기)
                          return (
                            <>
                              <td className="border border-gray-600 px-4 py-2 w-2/3"></td>
                              <td className="border border-gray-600 px-4 py-2 w-1/3"></td>
                            </>
                          );
                        }
                      })}
                    </tr>
                  );
                }
                return rows;
              })()}
            </tbody>
          </table>
        </div>
      ) : (
        <p className="text-gray-400 text-center">No data available</p>
      )}
    </div>
  );
}


// AOS, LOS, Max Elevation 정보 가져오는 함수
async function fetchPassList(satelliteName: string) {
  const response = await fetch(`/api/pass-list?sat=${satelliteName}`);
  if (!response.ok) throw new Error("Failed to fetch pass list");
  return response.json();
}

// 위성 정보 창
export function GS_SatelliteInfo({ satelliteName, className = "" }) {
  const [satelliteInfo, setSatelliteInfo] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (!satelliteName) return;
    
    const updateData = async () => {
      try {
        const data = await fetchSatelliteInfo(satelliteName);
        setSatelliteInfo(data);
        setLoading(false);
      } catch (err) {
        setError(err.message);
        setLoading(false);
      }
    };

    updateData();
  }, [satelliteName]);

  return (
    <div className={`${className} p-4 bg-gray-900 border border-blue-500 rounded-lg`}>
      <h2 className="text-lg font-bold text-white mb-2">{satelliteName} Info</h2>
      {loading ? (
        <p className="text-gray-400">Loading...</p>
      ) : error ? (
        <p className="text-red-500">{error}</p>
      ) : (
        <div className="text-white">
          <p>Inclination: {satelliteInfo.inclination}°</p>
          <p>RAAN: {satelliteInfo.raan}°</p>
          <p>Eccentricity: {satelliteInfo.eccentricity}</p>
          <p>Argument of Perigee: {satelliteInfo.argPerigee}°</p>
          <p>Mean Anomaly: {satelliteInfo.meanAnomaly}°</p>
          <p>Semi-Major Axis: {satelliteInfo.semiMajorAxis} km</p>
          <p>Altitude: {satelliteInfo.altitude} km</p>
          <p>Latitude: {satelliteInfo.latitude}°</p>
          <p>Longitude: {satelliteInfo.longitude}°</p>
        </div>
      )}
    </div>
  );
}

// 패스 리스트 창
export function GS_PassList({ satelliteName, className = "" }) {
  const [passList, setPassList] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (!satelliteName) return;

    const updateData = async () => {
      try {
        const data = await fetchPassList(satelliteName);
        setPassList(data);
        setLoading(false);
      } catch (err) {
        setError(err.message);
        setLoading(false);
      }
    };

    updateData();
  }, [satelliteName]);

  return (
    <div className={`${className} p-4 bg-gray-900 border border-blue-500 rounded-lg`}>
      <h2 className="text-lg font-bold text-white mb-2">{satelliteName} Pass List</h2>
      {loading ? (
        <p className="text-gray-400">Loading...</p>
      ) : error ? (
        <p className="text-red-500">{error}</p>
      ) : (
        <div className="overflow-auto max-h-80">
          <table className="w-full text-white border-collapse">
            <thead className="sticky top-0 bg-gray-700">
              <tr>
                <th className="border border-gray-600 px-4 py-2">AOS</th>
                <th className="border border-gray-600 px-4 py-2">LOS</th>
                <th className="border border-gray-600 px-4 py-2">Max Elevation</th>
              </tr>
            </thead>
            <tbody>
              {passList.length > 0 ? (
                passList.slice(0, 20).map((pass, index) => (
                  <tr key={index} className="hover:bg-gray-600">
                    <td className="border border-gray-600 px-4 py-2 text-center">{pass.aos}</td>
                    <td className="border border-gray-600 px-4 py-2 text-center">{pass.los}</td>
                    <td className="border border-gray-600 px-4 py-2 text-center">{pass.maxElevation}°</td>
                  </tr>
                ))
              ) : (
                <tr>
                  <td colSpan={3} className="text-center py-4 text-gray-400">No data available</td>
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
    <div className="grid grid-cols-[1.0fr_1.1fr_1.1fr] grid-rows-[0.9fr_1.1fr] gap-4 p-4 w-full h-screen">
      {/* TLE Table (col 1-2, row 1) */}
      <div className="col-span-2 row-span-1 overflow-hidden">
        <GS_TLETable
          className="relative p-6 bg-opacity-80 bg-gray-900 
                     border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                     rounded-lg backdrop-blur-md flex flex-col w-full h-full overflow-auto"
        />
      </div>

      {/* Satellite Info (col 1, row 2) */}
      <div className="col-start-1 col-span-1 row-start-2 flex">
        <GS_PassList
          className="relative p-6 bg-opacity-80 bg-gray-900 
                     border border-yellow-500 shadow-[0_0_10px_rgba(255,255,0,0.6)] 
                     rounded-lg backdrop-blur-md flex flex-col w-full h-full overflow-auto"
        />

      </div>

      {/* Satellite Pass List (col 2, row 2) */}
      <div className="col-start-2 col-span-1 row-start-2 flex">
        <GS_SatelliteInfo
          className="relative p-6 bg-opacity-80 bg-gray-900 
                     border border-green-500 shadow-[0_0_10px_rgba(0,255,0,0.6)] 
                     rounded-lg backdrop-blur-md flex flex-col w-full h-full"
        />
      </div>

      {/* Beacon Table (col 3, row 1-2) */}
      <div className="col-start-3 col-span-1 row-span-2 flex">
        <GS_BeaconTable
          className="relative p-6 bg-opacity-80 bg-gray-900 
                     border border-blue-500 shadow-[0_0_10px_rgba(0,255,255,0.6)] 
                     rounded-lg backdrop-blur-md flex flex-col w-full h-full"
        />
      </div>
    </div>
  );
}

