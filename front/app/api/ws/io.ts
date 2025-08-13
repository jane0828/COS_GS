
interface ParsedMessage {
    client: string;
    reqtype: string;
    data: any;
  }
  
  export function WS_ParameterRequest(
    reqtype: string,
    data: Record<string, any>
  ): string {
    return JSON.stringify({
      client: "gs634",
      reqtype: reqtype,
      data: data,
    })
  }
  
  export function WS_ParameterReceive(eventData: string): ParsedMessage | null {
    try {
      const msg = JSON.parse(eventData);
  
      if (
        msg.client === "client634" &&
        typeof msg.reqtype === "string" &&
        msg.data
      ) {
        return {
          client: msg.client,
          reqtype: msg.reqtype,
          data: msg.data,
        };
      }
      else if (
        msg.client === "client634" &&
        msg.err
      ) {
        console.error("Received Error:", msg.err);
      }
    } catch (err) {
      console.error("WebSocket JSON parse error:", err);
    }
  
    return null;
  }