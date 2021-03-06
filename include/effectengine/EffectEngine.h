#pragma once

// Qt includes
#include <QObject>
#include <QString>

// Json includes
#include <json/value.h>

// Hyperion includes
#include <hyperion/Hyperion.h>

// Effect engine includes
#include <effectengine/EffectDefinition.h>
#include <effectengine/ActiveEffectDefinition.h>
#include <utils/Logger.h>

// pre-declarioation
class Effect;
typedef struct _ts PyThreadState;

class EffectEngine : public QObject
{
	Q_OBJECT

public:
	EffectEngine(Hyperion * hyperion, const Json::Value & jsonEffectConfig);
	virtual ~EffectEngine();

	const std::list<EffectDefinition> & getEffects() const;
	
	const std::list<ActiveEffectDefinition> & getActiveEffects();

	static bool loadEffectDefinition(const QString & path, const QString & effectConfigFile, EffectDefinition &effectDefinition);

public slots:
	/// Run the specified effect on the given priority channel and optionally specify a timeout
	int runEffect(const std::string &effectName, int priority, int timeout = -1);

	/// Run the specified effect on the given priority channel and optionally specify a timeout
	int runEffect(const std::string &effectName, const Json::Value & args, int priority, int timeout = -1);

	/// Clear any effect running on the provided channel
	void channelCleared(int priority);

	/// Clear all effects
	void allChannelsCleared();

private slots:
	void effectFinished(Effect * effect);

private:
	/// Run the specified effect on the given priority channel and optionally specify a timeout
	int runEffectScript(const std::string &script, const std::string &name, const Json::Value & args, int priority, int timeout = -1);

private:
	Hyperion * _hyperion;

	std::list<EffectDefinition> _availableEffects;

	std::list<Effect *> _activeEffects;

	std::list<ActiveEffectDefinition> _availableActiveEffects;

	PyThreadState * _mainThreadState;

	Logger * _log;
};
