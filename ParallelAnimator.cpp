#include "ParallelAnimator.h"

void ParallelAnimator::nextAnimation() {
    ++m_currentIndex;
	if (m_currentIndex < m_animations.size()) {
		m_currentAnimation = m_animations[m_currentIndex].get();
		for (size_t i = 0; i < m_animations.size(); ++i) {
			m_animations[i]->start();
		}
		m_nextTransition = m_nextTransition + m_currentAnimation->duration();
	}
	else {
		m_currentIndex = -1;
		m_currentAnimation = nullptr;
	}
}


void ParallelAnimator::tick(float_t dt) {
	if (m_currentIndex >= 0) {
		float_t lastTime = m_currentTime;
		m_currentTime += dt;

		// If our current time surpasses the next transition time, we need to tick
		// both the active animation (up to the transition time), and the subsequent animation
		// (by the amount we exceeded the transition time).
		if (m_currentTime >= m_nextTransition) {
			m_currentAnimation->tick(m_nextTransition - lastTime);
			float_t overTime = m_currentTime - m_nextTransition;
			nextAnimation();
			if (m_currentAnimation != nullptr) {
				m_currentAnimation->tick(overTime);
			}
		}
		else {
			for (size_t i = 0; i < m_animations.size(); ++i) {
				m_animations[i]->tick(dt);
			}
		}
	}
}

void ParallelAnimator::start() {
    m_currentTime = 0;
    nextAnimation();
}
