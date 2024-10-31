#pragma once

#include "AbstractMaterial.h"

namespace xe {
class KdMaterial : public AbstractMaterial<KdMaterial> {
    public:
        KdMaterial(const glm::vec4 &Kd);

        static void init();
        void bind() const override;
        void unbind() const override;

    private:
        const glm::vec4 Kd_;

        // bool is not perceived by shader, so
        // I had to replace with int (maybe the problem is about my computer).
        int use_vertex_colors_;
    };
    
}