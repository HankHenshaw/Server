#include "async.h"
#include "observer.h"
#include <string>
#include <memory>
#include <iostream>

namespace async
{
    Subject subj;
    bool isInit = false;
    bool isCustomBlock = false;
    size_t bulkSize;
    std::string buffer;

    void init()
    {
        subj.AddSub(std::make_unique<FileObserver>(subj));
        subj.AddSub(std::make_unique<FileObserver>(subj));
        subj.AddSub(std::make_unique<CoutObserver>(subj));
    }

    handle_t connect(std::size_t bulk)
    {
        if(!isInit)
        {
            bulkSize = bulk;
            init();
            isInit = true;
        }
        std::string *str = new std::string;
        str->reserve(bulk);
        return str;
    }

    void receive(handle_t handle, const char *data, std::size_t size)
    {
        size_t counter = 0;

        if(!buffer.empty())
        {
            *static_cast<std::string*>(handle) = buffer;
            counter = static_cast<std::string*>(handle)->size();
        }

        for(size_t i = 0; i < size; ++i)
        {
            if(data[i] == '\n')
            {
                continue;
            } else if(data[i] == '{') {
                if(!static_cast<std::string*>(handle)->empty())
                {
                    subj.AddBuffer(static_cast<std::string*>(handle));
                    static_cast<std::string*>(handle)->clear();
                    counter = 0;
                }
                isCustomBlock = true;
                continue;
            } else if(data[i] == '}') {
                if(isCustomBlock) {
                    subj.AddBuffer(static_cast<std::string*>(handle));
                    static_cast<std::string*>(handle)->clear();
                    isCustomBlock = false;
                    counter = 0;
                }
                continue;
            }

            static_cast<std::string*>(handle)->push_back(data[i]);
            ++counter;

            if(bulkSize == counter && !isCustomBlock)
            {
                subj.AddBuffer(static_cast<std::string*>(handle));
                static_cast<std::string*>(handle)->clear();
                counter = 0;
            } else {
                buffer = *static_cast<std::string*>(handle);
            }
        }

    }

    void disconnect(handle_t handle)
    {
        if(!buffer.empty())
        {
            subj.AddBuffer(&buffer);
            buffer.clear();
        }
        delete static_cast<std::string*>(handle);
    }
}
