#pragma once

// std
#include <stdint.h>

// sickl header
#include <SiCKL.h>

// OMLT
#include "Enums.h"

using namespace SiCKL;

namespace OMLT
{
	class RestrictedBoltzmannMachine;
	class ContrastiveDivergence
	{
		struct ModelConfig
		{
			uint32_t MinibatchSize;
			uint32_t VisibleUnits;
			ActivationFunction VisibleType;
			uint32_t HiddenUnits;
			ActivationFunction HiddenType;
		};

		struct TrainingConfig
		{
			float LearningRate;
			float Momentum;
			float L1Regularization;
			float L2Regularization;
			float VisibleDropout;
			float HiddenDropout;

			TrainingConfig() 
				: LearningRate(0.0f)
				, Momentum(0.0f)
				, L1Regularization(0.0f)
				, L2Regularization(0.0f)
				, VisibleDropout(0.0f)
				, HiddenDropout(0.0f)
			{ }
		};

	public:
		ContrastiveDivergence(const ModelConfig);
		~ContrastiveDivergence();

		void SetTrainingConfig(const TrainingConfig&);

		void Train(const OpenGLBuffer2D&);

		float GetLastReconstructionError();
		float GetReconstructionError(const OpenGLBuffer2D&);

		// get a new RBM object dumped from GPU memory
		RestrictedBoltzmannMachine* GetRestrictedBoltzmannMachine() const;

		/// these methods are used by VisualRBM for real-time visualization
		bool DumpLastVisible(float* image, float* recon);
		bool DumpLastHidden(float* activations);
		bool DumpLastWeights(float* weights);

	private:
		ModelConfig _model_config;
		TrainingConfig _training_config;
		// we need to recompile the shaders if the training config changes
		bool _recompile_required;

		// texture buffers
		OpenGLBuffer2D _visible_dropout_random0;
		OpenGLBuffer2D _visible_dropout_random1;
		OpenGLBuffer2D _hidden_dropout_random0;
		OpenGLBuffer2D _hidden_dropout_random1;
		OpenGLBuffer2D _enabled_visible;
		OpenGLBuffer2D _enabled_hidden;

		OpenGLBuffer2D _visible;
		OpenGLBuffer2D _hidden;
		OpenGLBuffer2D _hidden_states;
		OpenGLBuffer2D _hidden_random0;
		OpenGLBuffer2D _hidden_random1;
		OpenGLBuffer2D _visible_prime;
		OpenGLBuffer2D _hidden_prime;

		OpenGLBuffer2D _weights0;
		OpenGLBuffer2D _weights1;
		OpenGLBuffer2D _delta_weights0;
		OpenGLBuffer2D _delta_weights1;
		OpenGLBuffer2D _error;

		OpenGLProgram* _calc_enabled_visible;
		OpenGLProgram* _calc_enabled_hidden;
		OpenGLProgram* _copy_visible;
		OpenGLProgram* _calc_hidden;
		OpenGLProgram* _calc_hidden_states;
		OpenGLProgram* _calc_visible;
		OpenGLProgram* _update_weights;
		OpenGLProgram* _calc_error;

		// kernel sources definitions
#		include "ContrastiveDivergenceKernels.h"
		// recompile kernel programs as necessary
		void DeleteKernels();
		void BuildKernels();
		void AllocateTextures(float* weight_buffer);
		/** Training Methods **/
/*
		void CalcEnabledUnits();
		void CalcVisibleCopy(OpenGLBuffer2D& source, OpenGLBuffer2D& destination);
		void CalcHiddenProbs(OpenGLBuffer2D& visible, OpenGLBuffer2D& hidden_probs);
		void CalcHiddenStates(OpenGLBuffer2D& probs, OpenGLBuffer2D& states);
		void CalcVisiblePrime(OpenGLBuffer2D& hidden, OpenGLBuffer2D& visible);
		void CalcWeights();
		float CalcError(OpenGLBuffer2D& visible, OpenGLBuffer2D& reconstruction);
*/
	};
}