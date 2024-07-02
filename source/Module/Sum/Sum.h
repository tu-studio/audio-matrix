#ifndef SUM_H
#define SUM_H

#include <Module.h>
#include <SumConfig.h>
class Sum: public Module {
    public:
        Sum() = delete;
        Sum(std::shared_ptr<SumConfig> config, std::shared_ptr<lo::ServerThread> osc_server);
        ~Sum() = default;

        size_t initialize(size_t input_channels) override;
        void process(AudioBufferF &buffer, size_t nframes) override;

    private:
        std::shared_ptr<SumConfig> m_config;
};

#endif //SUM_H