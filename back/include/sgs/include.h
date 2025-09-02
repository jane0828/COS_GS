#pragma once
#ifndef _SGS_INCLUDE_H_
#define _SGS_INCLUDE_H_
#ifndef __cplusplus
#define __cplusplus
#endif
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <map>
#include <vector>


#define GS_FTP_INTERNAL_USE 1
#define DEFAULT_CHUNK_SIZE  100
#define FTP_CALLBACK_REQUEST_TOTAL_CHUNKS	1
#define FTP_CALLBACK_REQUEST_CURRENT_CHUNKS	2
#define FTP_CALLBACK_REQUEST_CHUNKSIZE		3
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
#endif
// #include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <mysql/mysql.h>
#include <arpa/inet.h>

#include <csp/csp.h>
#include <csp/csp_error.h>
#include <csp/csp_endian.h>
#include <csp/csp_rtable.h>
#include <csp/csp_endian.h>
#include <csp_io.h>
#include <csp/arch/csp_time.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_system.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/drivers/usart.h>
#include <csp/switch.h>
#include <csp/delay.h>
#include <csp/csp_endian.h>
#include <csp/csp_buffer.h>

#include <gs/csp/csp.h>
#include <gs/csp/port.h>
#include <gs/ftp/client.h>
#include <gs/ftp/types.h>
#include <gs/ftp/internal/types.h>
#include <gs/util/crc32.h>
#include <gs/util/string.h>
#include <gs/util/crc32.h>
#include <gs/util/clock.h>
#include <gs/util/log.h>
#include <gs/util/vmem.h>
#include <gs/param/rparam.h>
#include <gs/param/types.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui_internal.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>

#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <SGP4.h>

// #include <object/arcball.h>
// #include <object/circle.h>
// #include <object/cone.h>
// #include <object/cube.h>
// #include <object/keyframe.h>
// #include <object/mass.h>
// #include <object/Mesh.h>
// #include <object/rectangle.h>
// #include <object/spline.h>
// #include <object/shader.h>

#include <sgs/tc/adcs.h>
#include <sgs/tc/cmd.h>
#include <sgs/tc/eps.h>
#include <sgs/tc/es.h>
#include <sgs/tc/fm.h>
#include <sgs/tc/gps.h>
#include <sgs/tc/mtq.h>
#include <sgs/tc/pay.h>
#include <sgs/tc/rwa.h>
#include <sgs/tc/snsr.h>
#include <sgs/tc/stx.h>
#include <sgs/tc/to.h>
#include <sgs/tc/ts.h>
#include <sgs/tc/utrx.h>

#include <sgs/baseline/io_filesys.h>
#include <sgs/baseline/io_shell.h>
#include <sgs/baseline/io_rs232.h>
#include <sgs/baseline/io_ip.h>
#include <sgs/baseline/flowcontrol.h>

#include <sgs/backend/gstrx.h>
#include <sgs/backend/satelliteobject.h>
#include <sgs/backend/rotator.h>
#include <sgs/backend/radial.h>
#include <sgs/backend/udp.h>

#include <sgs/frontend/autopilot.h>
#include <sgs/frontend/ping.h>
#include <sgs/frontend/beacon.h>
#include <sgs/frontend/fswcmd.h>
#include <sgs/frontend/syscmd.h>
#include <sgs/frontend/ftp.h>
#include <sgs/frontend/model.h>
#include <sgs/frontend/rendering.h>
#include <sgs/frontend/tle.h>

#include <sgs/ui/cmdwindow.h>
#include <sgs/ui/consolewindow.h>
#include <sgs/ui/ftpwindow.h>
#include <sgs/ui/propagatorwindow.h>
#include <sgs/ui/rotatorwindow.h>
#include <sgs/ui/setupinfowindow.h>
#include <sgs/ui/tleinfowindow.h>
#include <sgs/ui/tlmwindow.h>
#include <sgs/ui/trxparamwindow.h>
#include <sgs/ui/trxwindow.h>


#endif