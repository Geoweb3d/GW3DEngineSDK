/*!
	@file
	@author		Albert Semenov
	@date		01/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ProgressBar.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"
#include "MyGUI_SkinManager.h"

namespace MyGUI
{

	const size_t PROGRESS_AUTO_WIDTH = 200;
	const size_t PROGRESS_AUTO_RANGE = 1000;
	const float PROGRESS_AUTO_COEF = 400;

	ProgressBar::ProgressBar() :
		mTrackWidth(1),
		mTrackStep(0),
		mTrackMin(0),
		mRange(0),
		mStartPosition(0),
		mEndPosition(0),
		mAutoPosition(0.0f),
		mAutoTrack(false),
		mFillTrack(false),
		mClient(nullptr)
	{
	}

	void ProgressBar::initialiseOverride()
	{
		Base::initialiseOverride();

		assignWidget(mClient, "TrackPlace");

		if (nullptr == mClient)
		{
			//OBSOLETE
			assignWidget(mClient, "Client");

			if (nullptr == mClient)
				mClient = this;
		}

		if (isUserString("TrackSkin"))
			mTrackSkin = getUserString("TrackSkin");
		if (isUserString("TrackWidth"))
			mTrackWidth = utility::parseValue<int>(getUserString("TrackWidth"));
		//OBSOLETE
		if (isUserString("TrackMin"))
			mTrackMin = utility::parseValue<int>(getUserString("TrackMin"));
		else
			mTrackMin = mTrackWidth;
		if (isUserString("TrackStep"))
			mTrackStep = utility::parseValue<int>(getUserString("TrackStep"));
		if (isUserString("TrackFill"))
			mFillTrack = utility::parseValue<bool>(getUserString("TrackFill"));

		if (!isUserString("TrackStep"))
			mTrackStep = mTrackWidth;

		if (1 > mTrackWidth)
			mTrackWidth = 1;
	}

	void ProgressBar::shutdownOverride()
	{
		mClient = nullptr;

		Base::shutdownOverride();
	}

	void ProgressBar::setProgressRange(size_t _range)
	{
		if (mAutoTrack) return;
		mRange = _range;
		if (mEndPosition > mRange) mEndPosition = mRange;
		if (mStartPosition > mRange) mStartPosition = mRange;
		updateTrack();
	}

	void ProgressBar::setProgressPosition(size_t _pos)
	{
		if (mAutoTrack) return;
		mEndPosition = _pos;
		if (mEndPosition > mRange) mEndPosition = mRange;
		updateTrack();
	}

	void ProgressBar::setProgressAutoTrack(bool _auto)
	{
		if (mAutoTrack == _auto) return;
		mAutoTrack = _auto;

		if (mAutoTrack)
		{
			Gui::getInstance().eventFrameStart += newDelegate(this, &ProgressBar::frameEntered);
			mRange = PROGRESS_AUTO_RANGE;
			mEndPosition = mStartPosition = 0;
			mAutoPosition = 0.0f;
		}
		else
		{
			Gui::getInstance().eventFrameStart -= newDelegate(this, &ProgressBar::frameEntered);
			mRange = mEndPosition = mStartPosition = 0;
		}
		updateTrack();
	}

	void ProgressBar::frameEntered(float _time)
	{
		if (!mAutoTrack) return;
		mAutoPosition += (PROGRESS_AUTO_COEF * _time);
		size_t pos = (size_t)mAutoPosition;

		if (pos > (mRange + PROGRESS_AUTO_WIDTH)) mAutoPosition = 0.0f;

		if (pos > mRange) mEndPosition = mRange;
		else mEndPosition = size_t(mAutoPosition);

		if (pos < PROGRESS_AUTO_WIDTH) mStartPosition = 0;
		else mStartPosition = pos - PROGRESS_AUTO_WIDTH;

		updateTrack();
	}

	void ProgressBar::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ProgressBar::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateTrack();
	}

	void ProgressBar::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateTrack();
	}

	void ProgressBar::updateTrack()
	{
		// ?????? ????????????
		if ((0 == mRange) || (0 == mEndPosition))
		{
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				(*iter)->setVisible(false);
			}
			return;
		}

		// ?????? ?????????????? ??????????????
		if (mFillTrack)
		{
			if (mVectorTrack.empty())
			{
				Widget* widget = mClient->createWidget<Widget>(mTrackSkin, IntCoord(), Align::Left | Align::VStretch);
				mVectorTrack.push_back(widget);
			}
			else
			{
				// ???????????? ???????????????????? ?? ???????????? ???????? ??????????
				VectorWidgetPtr::iterator iter = mVectorTrack.begin();
				(*iter)->setVisible(true);
				(*iter)->setAlpha(ALPHA_MAX);

				// ?????? ?????????????? ???? ?????????????? ????????????????
				++iter;
				for (; iter != mVectorTrack.end(); ++iter)
				{
					(*iter)->setVisible(false);
				}
			}

			Widget* wid = mVectorTrack.front();

			// ?????????????????? ??????????
			if ((0 == mStartPosition) && (mRange == mEndPosition))
			{
				setTrackPosition(wid, 0, 0, getClientWidth(), getClientHeight());
			}
			// ????
			else
			{
				int pos = (int)mStartPosition * (getClientWidth() - mTrackMin) / (int)mRange;
				setTrackPosition(wid, pos, 0, ((int)mEndPosition * (getClientWidth() - mTrackMin) / (int)mRange) - pos + mTrackMin, getClientHeight());
			}

			return;
		}

		// ?????????????? ?????????????????? ?????????????? ?????? ??????????
		int width = getClientWidth() - mTrackWidth + mTrackStep;
		int count = width / mTrackStep;
		int ost = (width % mTrackStep);
		if (ost > 0)
		{
			width += mTrackStep - ost;
			count ++;
		}

		while ((int)mVectorTrack.size() < count)
		{
			Widget* widget = mClient->createWidget<Widget>(mTrackSkin, IntCoord(/*(int)mVectorTrack.size() * mTrackStep, 0, mTrackWidth, getClientHeight()*/), Align::Left | Align::VStretch);
			widget->setVisible(false);
			mVectorTrack.push_back(widget);
		}

		// ?????? ??????????
		if ((0 == mStartPosition) && (mRange == mEndPosition))
		{
			int pos = 0;
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				(*iter)->setAlpha(ALPHA_MAX);
				(*iter)->setVisible(true);
				setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				pos++;
			}
		}
		// ????, ???????????????? ??????????????
		else
		{
			// ?????????????? ???? ??????????
			int hide_pix = (width * (int)mStartPosition / (int)mRange);
			int hide_count = hide_pix / mTrackStep;
			// ?????????????? ??????????
			int show_pix = (width * (int)mEndPosition / (int)mRange);
			int show_count = show_pix / mTrackStep;

			int pos = 0;
			for (VectorWidgetPtr::iterator iter = mVectorTrack.begin(); iter != mVectorTrack.end(); ++iter)
			{
				if (0 > show_count)
				{
					(*iter)->setVisible(false);
				}
				else if (0 == show_count)
				{
					(*iter)->setAlpha((float)(show_pix % mTrackStep) / (float)mTrackStep);
					(*iter)->setVisible(true);
					setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
				}
				else
				{
					if (0 < hide_count)
					{
						(*iter)->setVisible(false);
					}
					else if (0 == hide_count)
					{
						(*iter)->setAlpha(1.0f - ((float)(hide_pix % mTrackStep) / (float)mTrackStep));
						(*iter)->setVisible(true);
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
					else
					{
						(*iter)->setAlpha(ALPHA_MAX);
						(*iter)->setVisible(true);
						setTrackPosition(*iter, pos * mTrackStep, 0, mTrackWidth, getClientHeight());
					}
				}
				hide_count --;
				show_count --;
				pos ++;
			}
		}
	}

	void ProgressBar::setTrackPosition(Widget* _widget, int _left, int _top, int _width, int _height)
	{
		if (mFlowDirection == FlowDirection::LeftToRight) _widget->setCoord(_left, _top, _width, _height);
		else if (mFlowDirection == FlowDirection::RightToLeft) _widget->setCoord(mClient->getWidth() - _left - _width, _top, _width, _height);
		else if (mFlowDirection == FlowDirection::TopToBottom) _widget->setCoord(_top, _left, _height, _width);
		else if (mFlowDirection == FlowDirection::BottomToTop) _widget->setCoord(_top, mClient->getHeight() - _left - _width, _height, _width);
	}

	int ProgressBar::getClientWidth()
	{
		return mFlowDirection.isHorizontal() ? mClient->getWidth() : mClient->getHeight();
	}

	int ProgressBar::getClientHeight()
	{
		return mFlowDirection.isHorizontal() ? mClient->getHeight() : mClient->getWidth();
	}

	void ProgressBar::setFlowDirection(FlowDirection _value)
	{
		mFlowDirection = _value;
		updateTrack();
	}

	void ProgressBar::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "Range")
			setProgressRange(utility::parseValue<size_t>(_value));
		else if (_key == "RangePosition")
			setProgressPosition(utility::parseValue<size_t>(_value));
		else if (_key == "AutoTrack")
			setProgressAutoTrack(utility::parseValue<bool>(_value));
		else if (_key == "FlowDirection")
			setFlowDirection(utility::parseValue<FlowDirection>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	size_t ProgressBar::getProgressRange() const
	{
		return mRange;
	}

	size_t ProgressBar::getProgressPosition() const
	{
		return mEndPosition;
	}

	bool ProgressBar::getProgressAutoTrack() const
	{
		return mAutoTrack;
	}

	FlowDirection ProgressBar::getFlowDirection() const
	{
		return mFlowDirection;
	}

	void ProgressBar::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void ProgressBar::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void ProgressBar::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

} // namespace MyGUI
