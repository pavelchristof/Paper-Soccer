#include "recentlysaved.hpp"

namespace ps {

void RecentlySaved::load()
{
	QSettings settings;
	setStringList(settings.value(propertyName, QStringList{}).toStringList());
}

void RecentlySaved::save()
{
	QSettings settings;
	settings.setValue(propertyName, stringList());
}

void RecentlySaved::add(const QString& path)
{
	QStringList list = stringList();
	list.removeAll(path);
	list.prepend(path);
	while (list.size() > 10) {
		list.pop_back();
	}
	setStringList(list);
}

const char* RecentlySaved::propertyName = "recentlySaved";

} // namespace ps
