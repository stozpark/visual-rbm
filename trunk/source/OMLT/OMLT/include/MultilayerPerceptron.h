#pragma once

// std
#include <stdint.h>
#include <vector>

#include "Enums.h"

namespace OMLT
{
	class MultilayerPerceptron
	{
	public:

		void FeedForward(float* input_vector, float* output_vector);

		struct Layer
		{
			uint32_t inputs;
			uint32_t outputs;
			ActivationFunction function;
			float* biases;
			float** weights;

			Layer(uint32_t in_inputs, uint32_t in_outputs, ActivationFunction in_function);
			~Layer();
		};

		Layer* GetLayer(uint32_t index);
		bool AddLayer(Layer*);
		uint32_t LayerCount() const {return _layers.size();}

		std::string ToJSON() const;
		static MultilayerPerceptron& FromJSON(std::string in_JSON);

private: 
		std::vector<Layer*> _layers;
		std::vector<float*> _accumulations;
		std::vector<float*> _activations;
	};
}