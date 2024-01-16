
/// @brief - A minimalistic bsg server implementation

#include "Server.hh"
#include <core_utils/CoreException.hh>
#include <core_utils/log/Locator.hh>
#include <core_utils/log/PrefixedLogger.hh>
#include <core_utils/log/StdLogger.hh>
#include <functional>

#include "JumpMessage.hh"
#include "ScannedMessage.hh"
#include "VectorUtils.hh"
#include "VelocityMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace {
// https://stackoverflow.com/questions/11468414/using-auto-and-lambda-to-handle-signal
std::function<void(int)> sigIntProcessing;
void sigIntInterceptor(const int signal)
{
  sigIntProcessing(signal);
}
} // namespace

int main(int /*argc*/, char ** /*argv*/)
{
  // Create the logger.
  utils::log::StdLogger raw;
  raw.setLevel(utils::log::Severity::DEBUG);
  utils::log::PrefixedLogger logger("server", "main");
  utils::log::Locator::provide(&raw);

  try
  {
    bsgo::MessageType in0  = bsgo::MessageType::TARGET;
    bsgo::MessageType out0 = bsgo::MessageType::TARGET;

    std::ostringstream ostr0;
    utils::serialize(ostr0, in0);
    std::istringstream istr0(ostr0.str());
    utils::deserialize(istr0, out0);

    logger.info("in: " + bsgo::str(in0) + ", out: " + bsgo::str(out0));

    bsgo::MessageType in  = bsgo::MessageType::JUMP;
    bsgo::MessageType out = bsgo::MessageType::DOCK;

    std::ostringstream ostr;
    utils::serialize(ostr, in);
    std::istringstream istr(ostr.str());
    utils::deserialize(istr, out);

    logger.info("in: " + bsgo::str(in) + ", out: " + bsgo::str(out));

    bsgo::ScannedMessage in2(bsgo::Uuid{2});
    bsgo::ScannedMessage out2(bsgo::Uuid{36});

    std::ostringstream ostr2;
    ostr2 << in2;
    std::istringstream istr2(ostr2.str());
    istr2 >> out2;

    logger.info("in: " + bsgo::str(in2.type()) + "/" + bsgo::str(in2.asteroidEntityId())
                + ", out: " + bsgo::str(out2.type()) + "/" + bsgo::str(out2.asteroidEntityId()));

    bsgo::VelocityMessage in3(bsgo::Uuid{2}, Eigen::Vector3f(0.1f, -4.6f, 36.7f));
    bsgo::VelocityMessage out3(bsgo::Uuid{36}, Eigen::Vector3f::Ones());

    std::ostringstream ostr3;
    ostr3 << in3;
    std::istringstream istr3(ostr3.str());
    istr3 >> out3;

    logger.info("in: " + bsgo::str(in3.type()) + "/" + bsgo::str(in3.getShipEntityId()) + "/"
                + bsgo::str(in3.getAcceleration()) + ", out: " + bsgo::str(out3.type()) + "/"
                + bsgo::str(out3.getShipEntityId()) + "/" + bsgo::str(out3.getAcceleration()));

    bsgo::JumpMessage in4(bsgo::Uuid{2}, bsgo::Uuid{26}, bsgo::JumpState::CANCELLED);
    bsgo::JumpMessage out4(bsgo::Uuid{36}, bsgo::Uuid{14}, bsgo::JumpState::RUNNING, bsgo::Uuid{7});

    std::ostringstream ostr4;
    ostr4 << in4;
    std::istringstream istr4(ostr4.str());
    istr4 >> out4;

    logger.info("in: " + bsgo::str(in4.type()) + "/" + bsgo::str(in4.getShipDbId()) + "/"
                + bsgo::str(in4.getShipEntityId()) + "/"
                + std::to_string(static_cast<int>(in4.getJumpState()))
                + ", out: " + bsgo::str(out4.type()) + "/" + bsgo::str(out4.getShipDbId()) + "/"
                + bsgo::str(out4.getShipEntityId()) + "/"
                + std::to_string(static_cast<int>(out4.getJumpState())));

    bsgo::Server server;
    sigIntProcessing = [&server](const int /*signal*/) { server.requestStop(); };
    std::signal(SIGINT, sigIntInterceptor);

    constexpr auto DEFAULT_SERVER_PORT = 60000;
    server.run(DEFAULT_SERVER_PORT);
  }
  catch (const utils::CoreException &e)
  {
    logger.error("Caught internal exception while setting up application", e.what());
    return EXIT_FAILURE;
  }
  catch (const std::exception &e)
  {
    logger.error("Caught internal exception while setting up application", e.what());
    return EXIT_FAILURE;
  }
  catch (...)
  {
    logger.error("Unexpected error while setting up application");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
