export function hs2float(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readFloatBE(0);
  }
  
  export function hs2uint8(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readUInt8(0);
  }
  
  export function hs2uint16(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readUInt16BE(0);
  }
  
  export function hs2uint32(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readUInt32BE(0);
  }
  
  export function hs2int8(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readInt8(0);
  }
  
  export function hs2int16(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readInt16BE(0);
  }
  
  export function hs2int32(hexStr: string): number {
    const buf = Buffer.from(hexStr, 'hex');
    return buf.readInt32BE(0);
  }
  