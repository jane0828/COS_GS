#ifndef _SGS_INCLUDE_H_
// #include <sgs/include.h>
// #include <sgs/global.h>
#endif

#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>

SGS_GS100::SGS_GS100(SGS_CSP* _csp_handler, const char* _device, uint32_t _baudrate)
{
	int ec;
	this->console = new SGS_CONSOLE();
	this->console->set_catalog(SGS_DCS);

	if(!_csp_handler)
		return;
	this->csp_handler = _csp_handler;
	ec = csp_handler->add_ifc_kiss(_device, _baudrate);
	memset(tinst, 0, sizeof(tinst));
	memset(fname, 0, sizeof(fname));

	snprintf(fname[0], sizeof(fname[0]), ("./gs100_param0.bin"));
	snprintf(fname[1], sizeof(fname[1]), ("./gs100_param1.bin"));
	snprintf(fname[2], sizeof(fname[2]), ("./gs100_param2.bin"));
	snprintf(fname[3], sizeof(fname[3]), ("./gs100_param3.bin"));
	snprintf(fname[4], sizeof(fname[4]), ("./gs100_param4.bin"));
	snprintf(fname[5], sizeof(fname[5]), ("./gs100_param5.bin"));
}

SGS_GS100::~SGS_GS100()
{

}

int SGS_GS100::register_handle(SGS_WebSocketClient* _ws)
{
    if (!_ws)
        return SGS_ERROR_NULL;

    this->ws = _ws;

	// Register JSON handler functions using the macro
	REGISTER_JSON_HANDLER(this->ws, this, get_param_table_json);
	REGISTER_JSON_HANDLER(this->ws, this, get_param_json);
	REGISTER_JSON_HANDLER(this->ws, this, set_param_json);
	REGISTER_JSON_HANDLER(this->ws, this, save_table_json);
	REGISTER_JSON_HANDLER(this->ws, this, set_watchdog_json);

    return SGS_OK;
}

int SGS_GS100::set_csp_settings(uint8_t _node, uint8_t _timeout)
{
	this->node = _node;
	this->timeout = _timeout;
	return SGS_OK;
}

int SGS_GS100::get_param_table(uint8_t table_id)
{
	int error;
	uint16_t checksum = this->csp_handler->get_mcksum();
	error = gs_rparam_download_table_spec(&this->tinst[table_id], this->fname[table_id], this->node, table_id, this->timeout, &checksum);

	if(error == GS_OK)
		this->console->debug("rparam download success on table %d", table_id);
	else
	{
		this->console->error("rparam download failed on table %d", table_id);
		return error;
	}

	error = gs_rparam_get_full_table(&this->tinst[table_id], this->node, table_id, this->cksum, this->timeout);
	if(error == GS_OK)
		this->console->debug("rparam get full table success ont table %d", table_id);
	else
	{
		this->console->error("rparam get full table failed on table %d", table_id);
		return error;
	}

	// memset(this->tablebuf, 0, sizeof(this->tablebuf));

	// size_t bufsz = sizeof(this->tablebuf);

	// FILE* fp = open_memstream((char**)&this->tablebuf, &bufsz);
    // if (!fp) {
	// 	this->console->error("open memstream failed.");
    //     return SGS_ERROR_BUSY;
    // }

    // // 기존 gs_param_list_to_stream()을 이용하여 메모리로 데이터 저장
    // error = gs_param_list_to_stream(tinst, true, 0, fp);
    // fflush(fp);  // 데이터 버퍼 강제 저장
    // fclose(fp);  // 메모리 스트림 닫기 (buffer 사용 가능)

	if(error == GS_OK)
		this->console->debug("table update success.");
	else
	{
		this->console->error("table update error.");
	}

	return error;
}

int SGS_GS100::get_param_table_json(const Json::Value& data)
{
	if(!this->ws)
		return SGS_ERROR_NULL;
	int res = this->ws->CheckJSONError(data);
	if(res != SGS_OK)
		return res;

	JSONVAL2UINT8(table_id)
	res = this->get_param_table(table_id);

	Json::Value reply;
	reply["table_id"] = table_id;
	reply["ec"] = res;
	
	if(res == GS_OK)
		reply["data"] = this->fname[table_id];
	this->ws->SendMessage2Provider("gs100_get_param_table", reply);
	return res;
}

int SGS_GS100::get_param(uint8_t table_id, const char* name, void** value, uint16_t& value_size)
{
	int ec = 0, i;
	if (*value) {
		free(*value);
		*value = nullptr;
	}
	// ...

	for (i = 0; i < this->tinst[table_id].row_count; i++) {
		ec = strcmp(name, this->tinst[table_id].rows[i].name);
		if (ec == strlen(name))
			break;
	}

	if (!ec) {
		this->console->error("rparam get failed. did you update the table first?");
		return SGS_ERROR_NOT_FOUND;
	}

	const gs_param_table_row_t* row = &this->tinst[table_id].rows[i];
	value_size = row->size;
	*value = malloc(value_size);
	return gs_rparam_get(this->node, table_id, row->addr, row->type, this->cksum, this->timeout, *value, value_size);
}


int SGS_GS100::get_param_json(const Json::Value& data)
{
	if(!this->ws)
		return SGS_ERROR_NULL;
	int res = this->ws->CheckJSONError(data);
	if(res != SGS_OK)
		return res;

	JSONVAL2UINT8(table_id)
	JSONVAL2CSTR(name)
	void* val = nullptr;
	uint16_t sz = 0;

	res = this->get_param(table_id, name, &val, sz);


	Json::Value reply;
	reply["table_id"] = table_id;
	reply["name"] = name;

	if(res == GS_OK)
		reply["data"] = this->ws->MakeHexAsciiJson(sz, (uint8_t *)val);

	this->ws->SendMessage2Provider("gs100_get_param", reply);
	return res;
}

int SGS_GS100::set_param(uint8_t table_id, const char* name, void* value, uint16_t& value_size)
{
	int ec, i;
	if(!value)
		return SGS_ERROR_NULL;
	
	const gs_param_table_row_t* row;
	for(i = 0; i < this->tinst[table_id].row_count; i++)
	{
		ec = strcmp(name, this->tinst[table_id].rows[i].name);
		if(ec == strlen(name))
			break;
	}
	if(!ec)
	{
		this->console->error("rparam set failed. did you updated table first?");
		return SGS_ERROR_NOT_FOUND;
	}
	row = &this->tinst[table_id].rows[i];
	value_size = row->size;
	return gs_rparam_set(this->node, table_id, row->addr, row->type, this->cksum, this->timeout, value, value_size);
}

int SGS_GS100::set_param_json(const Json::Value& data) {
	if (!this->ws)
		return SGS_ERROR_NULL;

	int res = this->ws->CheckJSONError(data);
	if (res != SGS_OK)
		return res;

	JSONVAL2UINT8(table_id)
	JSONVAL2CSTR(name)
	JSONVAL2BIN(val_ptr, val_len)
	res = this->set_param(table_id, name, val_ptr, val_len);

	this->ws->SendReport2Provider(res);

	return res;
}


int SGS_GS100::set_watchdog()
{
	csp_packet_t * dummy = (csp_packet_t*)csp_buffer_get(0);
	this->console->info("GS100 watchdog is now 172800.");
	return csp_sendto(CSP_PRIO_NORM, this->node, GS100_PORT_WATCHDOG, GS100_PORT_WATCHDOG, CSP_O_NONE, dummy, this->timeout);
}

int SGS_GS100::set_watchdog_json(const Json::Value& data) {
	if (!this->ws)
		return SGS_ERROR_NULL;

	int res = this->ws->CheckJSONError(data);
	if (res != SGS_OK)
		return res;

	res = this->set_watchdog();

	this->ws->SendReport2Provider(res);

	return res;
}


int SGS_GS100::save_table(uint8_t table_id)
{
	int ec;
	ec = gs_rparam_save(this->node, this->timeout, table_id, table_id);
	if(ec == GS_OK)
		this->console->info("Param Table %hhu is now saved.", table_id);
	else
		this->console->error("Param Table %hhu failed to save.", table_id);

	return ec;
}

int SGS_GS100::save_table_json(const Json::Value& data) {
	if (!this->ws)
		return SGS_ERROR_NULL;

	int res = this->ws->CheckJSONError(data);
	if (res != SGS_OK)
		return res;

	JSONVAL2UINT8(table_id)
	res = this->save_table(table_id);

	this->ws->SendReport2Provider(res);

	return res;
}

int SGS_GS100::reboot()
{
	csp_packet_t * dummy = (csp_packet_t*)csp_buffer_get(0);
	this->console->info("GS100 watchdog is now 172800.");
	return csp_sendto(CSP_PRIO_NORM, this->node, GS100_PORT_REBOOT, GS100_PORT_REBOOT, CSP_O_NONE, dummy, this->timeout);
}

int SGS_GS100::reboot_json(const Json::Value& data)
{
	if (!this->ws)
	return SGS_ERROR_NULL;

	int res = this->ws->CheckJSONError(data);
	if (res != SGS_OK)
		return res;

	res = this->reboot();

	this->ws->SendReport2Provider(res);

	return res;
}