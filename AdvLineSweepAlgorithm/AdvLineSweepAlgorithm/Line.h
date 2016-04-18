#pragma once

class CLine
{
	private:
		static const UINT LOWEST_X_COORDINATE;
		static const UINT HIGHEST_X_COORDINATE;
		static const UINT LOWEST_Y_COORDINATE;
		static const UINT HIGHEST_Y_COORDINATE;

	private:
		SPACE_POINT m_ptUpper;
		SPACE_POINT m_ptLower;

	public:
		CLine(void);
    CLine(const CLine& data);
		virtual ~CLine(void);
		SPACE_POINT getUpperPoint();
		SPACE_POINT getLowerPoint();
		void SetUpperPoint(SPACE_POINT ptUpper);
		void SetLowerPoint(SPACE_POINT ptLower);
		bool PointLiesOnLine(SPACE_POINT ptPoint);
		bool PointLiesBetweenLine(SPACE_POINT ptPoint);
    bool operator ==(CLine objLine);
    bool operator !=(CLine objLine);

	private:
		SPACE_POINT GenerateRandomPoint();
    bool IsPointOnLine(SPACE_POINT ptPoint);
};
