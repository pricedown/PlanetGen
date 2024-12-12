#include <vector>
#include "geometry.h"

namespace pl {
    class Cubemap {
        public:
            Cubemap(const std::vector<std::string>& faces);

            unsigned int getTextureID();
            void drawSkybox();
        private:
            unsigned int textureID;
            unsigned int skyboxVAO, skyboxVBO;

            unsigned int loadTextures(vector<std::string> faces);
            void loadSkybox();
    };

}
