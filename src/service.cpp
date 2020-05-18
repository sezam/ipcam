#include "service.h"

IpcamSession::IpcamSession(io_service& io_service, string ipcam_name, string ipcam_url)
    : socket_(io_service), io_service_(io_service),
    ipcam_url_(ipcam_url), ipcam_name_(ipcam_name)
{
    cout << "ipcam (" << ipcam_name_ << "): open session for socket: " << socket().native_handle() << endl;
}

IpcamSession::~IpcamSession()
{
    socket().close();
    cout << "ipcam (" << ipcam_name_ << "): close session for socket: " << socket().native_handle() << endl;
}

tcp::socket& IpcamSession::socket()
{
    return socket_;
}

void IpcamSession::get_photo()
{
    int read_buf_len = 100;
    char* read_buf = (char*)malloc(read_buf_len);
    memset(read_buf, 0, read_buf_len);

    bool step = true;
    size_t io_len = 0;
    boost::system::error_code err = error::would_block;

    if (step)
    {
        io_len = socket_.read_some(buffer(read_buf, read_buf_len), err);
        step = (!err && io_len > 0);
        if (!step) cout << "ipcam (" << ipcam_name_ << "): read command error: " << err.message() << endl;
    }

    if (step)
    {
        step = (string(read_buf) == "GetPhoto");
        if (!step) cout << "ipcam (" << ipcam_name_ << "): unknow command: " << err.message() << endl;
    }

    cv::VideoCapture vcap;
    cv::Mat frame;
    if (step)
    {
        step = vcap.open(ipcam_url_);
        if (!step) cout << "ipcam (" << ipcam_name_ << "): open cam url error" << endl;
    }

    if (step)
    {
        step = vcap.read(frame);
        if (!step) cout << "ipcam (" << ipcam_name_ << "): frames not grabbed" << endl;
    }

    cv::Mat image = cv::Mat(GLOBALWIDTH, GLOBALHEIGTH, CV_8UC3);
    std::vector<uchar> buff;
    if (step)
    {
        cv::transpose(frame, image);
        cv::flip(image, image, 1);
        step = cv::imencode(".jpg", image, buff);
        if (!step) cout << "ipcam (" << ipcam_name_ << "): error convert frame to image" << endl;
    }

    void* base64 = nullptr;
    if (step)
    {
        base64 = malloc(buff.size() * 2);
        step = base64 != NULL;
        if (!step)  cout << "ipcam (" << ipcam_name_ << "): error memory allocate" << endl;
    }

    if (step)
    {
        size_t base64_len = base64::encode(base64, buff.data(), buff.size());
        string json = (boost::format(response_format) % ipcam_name_ % "GetPhoto" % "0" % "OK" % base64).str();
        size_t send_buf_len = json.length();

        unsigned char send_buf[8];
        send_buf[0] = 'p';
        send_buf[1] = 'a';
        send_buf[2] = 'k';
        send_buf[3] = 't';
        send_buf[4] = (send_buf_len >> 24) & 0xFF;
        send_buf[5] = (send_buf_len >> 16) & 0xFF;
        send_buf[6] = (send_buf_len >> 8) & 0xFF;
        send_buf[7] = (send_buf_len & 0xFF);
        io_len = socket_.write_some(buffer(send_buf, 8), err);
        step = (!err && io_len == 8);

        if(step) io_len = socket_.write_some(buffer(base64, base64_len), err);
        step = (!err && io_len == base64_len);

        if (!step)  cout << "ipcam (" << ipcam_name_ << "): send data error" << endl;
    }

    image.release();
    frame.release();
    free(base64);
    free(read_buf);
}

SocketServer::SocketServer(io_service& io_service, string ipcam_url, string ipcam_name, int sock)
    : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), sock), true),
    ipcam_url_(ipcam_url), ipcam_name_(ipcam_name), sem_(1)
{
    cout << "server (" << ipcam_name_ << "): create" << endl;
    start_accept();
}

SocketServer::~SocketServer()
{
    cout << "server (" << ipcam_name_ << "): destroy" << endl;
}

void SocketServer::start_accept()
{
    IpcamSession* session = new IpcamSession(io_service_, ipcam_name_, ipcam_url_);
    acceptor_.async_accept(session->socket(),
        boost::bind(&SocketServer::handle_accept, this, session, boost::asio::placeholders::error));
}

void SocketServer::handle_accept(IpcamSession* session, const boost::system::error_code& error)
{
    if (!error)
    {
        sem_.wait();
        session->get_photo();
        sem_.post();
    }
    else
    {
        cout << "server (" << ipcam_name_ << "): error accept connection: " << error.value() << endl;
        delete session;
    }

    start_accept();
}
