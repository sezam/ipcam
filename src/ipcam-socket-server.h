#pragma once
#pragma execution_character_set("utf-8")

#ifndef IPCAM_SOCKET_SERVER_H
#define IPCAM_SOCKET_SERVER_H

//system dependency
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#pragma warning( disable : 4996)
#endif

#define BOOST_NO_ANSI_APIS
#define __STDC_WANT_LIB_EXT1__ 1

#include <iostream>
#include <string>
#include <cstdint>
#include <fstream>
#include <limits>
using namespace std;

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Winsock2.h>
#include <Windows.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
using namespace boost::asio;
using boost::asio::ip::tcp;

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
using namespace boost::interprocess;

#include <boost/format.hpp>
#include <boost/beast.hpp>
using namespace boost::beast::detail;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
using boost::property_tree::ptree;

#include <boost/thread.hpp>

#include <opencv2/opencv.hpp>

#define GLOBALWIDTH 640
#define GLOBALHEIGTH 480

static const string response_format_photo = R"({"Device":"%s","Response":{"TypeResponse":"GetPhoto","Result":"%s","Text":"%s","Data":"<base64>%s</base64>"}})";

static const string response_format_status = R"({"Device":"%s","Response":{"TypeResponse":"GetStatus","Status":"%s","Enabled":"True"}})";


void load_settings(char* path, function<void(string, string, int)> action);

#endif
