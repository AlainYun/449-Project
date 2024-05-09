#pragma once
#include <memory>
#include "Animation.h"
#include "RotationAnimation.h"
#include "TranslationAnimation.h"
#include "PauseAnimation.h"

class ParallelAnimator {
private:
	std::vector<std::unique_ptr<Animation>> m_animations;
	int32_t m_currentIndex;
	Animation* m_currentAnimation;
	float_t m_nextTransition;
	void nextAnimation();
	float_t m_currentTime;
public:
	/**
	 * @brief Constructs an Animator that acts on the given object.
	 */
	ParallelAnimator() :
		m_currentIndex(-1),
		m_nextTransition(0),
		m_currentTime(0),
		m_currentAnimation(nullptr) {
	}

	/**
	 * @brief Add an Animation to the end of the animation sequence.
	 */
	void addAnimation(std::unique_ptr<Animation> animation) {
		m_animations.emplace_back(std::move(animation));
	}

	/**
	 * @brief Activate the Animator, causing its active animation to receive future tick() calls.
	 */
	void start();

	/**
	 * @brief Advance the animation sequence by the given time interval, in seconds.
	 */
	void tick(float_t dt);

};