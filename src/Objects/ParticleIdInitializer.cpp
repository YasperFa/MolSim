#include <mutex>
#include "ParticleIdInitializer.h"

int id = 1;
std::mutex id_mutex;

int ParticleIdInitializer::getNewId(){
        id_mutex.lock();
        int newID = id++;
        id_mutex.unlock();
        return newID;
    }

void ParticleIdInitializer::reset(){
        id = 1;
}