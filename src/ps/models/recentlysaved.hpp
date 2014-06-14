#ifndef PS_MODELS_RECENTLYSAVED_HPP
#define PS_MODELS_RECENTLYSAVED_HPP

#include <QtCore/QStringListModel>
#include <QtCore/QSettings>

namespace ps
{

class RecentlySaved : public QStringListModel
{
	Q_OBJECT

public:

	/**
	 * Loads the list using QSettings.
	 */
	void load();

	/**
	 * Saves the list using QSettings.
	 */
	void save();

	/**
	 * Adds a new item to the begining of the list.
	 */
	void add(const QString& filePath);

private:
	static const char* propertyName;
};

}

#endif // PS_MODELS_RECENTLYSAVED_HPP
