#pragma once


namespace MG {

	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f)
			: m_Time(time)
		{

		}
		
		//相当于实现了强转float
		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};

}	//end MG