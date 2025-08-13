#include <dcs/dcs.h>
#include <utl/utl.h>

SGS_KTA223::SGS_KTA223(const char* _devname, uint32_t _baud)
	: SGS_RS232(_devname, _baud)
{
    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_DCS);
}

SGS_KTA223::~SGS_KTA223()
{
    this->Close();
}

int SGS_KTA223::register_handle(SGS_WebSocketClient* _ws)
{
    if (!_ws)
        return SGS_ERROR_NULL;

    this->ws = _ws;

	// Register JSON handler functions using the macro
	REGISTER_JSON_HANDLER(this->ws, this, switch_to_rx_json);
	REGISTER_JSON_HANDLER(this->ws, this, switch_to_tx_json);

    return SGS_OK;
}

int SGS_KTA223::switch_to_rx()
{
		
	// Turn on relay 2 only: 00000010
	if (this->Write("@00WR2\r", 7) < 1){
		this->console->error("Fail to turn on RX");
		return SGS_ERROR_NOT_CONNECTED;
	}
	this->console->debug("SWITCH : RX");

	return SGS_OK;
}

int SGS_KTA223::switch_to_rx_json(const Json::Value& data)
{
	if (!this->ws)
    return SGS_ERROR_NULL;

    int res = ws->CheckJSONError(data);
    if (res != SGS_OK)
        return res;

	res = this->switch_to_rx();
	return res;
}

int SGS_KTA223::switch_to_tx()
{
	// Turn on relay 2 only: 00000100
	if (this->Write("@00WR4\r", 7) < 1) {
		this->console->error("Fail to turn on TX");
		return SGS_ERROR_NOT_CONNECTED;
	}
	this->console->debug("SWITCH : TX");
	return SGS_OK;
}


int SGS_KTA223::switch_to_tx_json(const Json::Value& data)
{
	if (!this->ws)
    return SGS_ERROR_NULL;

    int res = ws->CheckJSONError(data);
	if (res != SGS_OK)
        return res;

	res = this->switch_to_tx();
	return res;
}