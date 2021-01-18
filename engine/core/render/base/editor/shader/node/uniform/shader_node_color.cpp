#include "shader_node_color.h"
#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include "engine/core/util/StringUtil.h"
#include <QMenu>

using namespace Echo;

namespace DataFlowProgramming
{
    ColorDataModel::ColorDataModel()
        : ShaderUniformDataModel()
    {
        m_outputs.resize(2);

        m_colorSelect = new QT_UI::QColorSelect();
        m_colorSelect->setFixedSize(75, 75);
        m_colorSelect->setDrawText(false);

        QObject::connect(m_colorSelect, SIGNAL(Signal_ColorChanged()), this, SLOT(onColorEdited()));
		QObject::connect(m_setAsParameter, SIGNAL(triggered()), this, SLOT(onSetAsParameter()));
		QObject::connect(m_setAsConstant, SIGNAL(triggered()), this, SLOT(onSetAsConstant()));

        m_uniformConfig = EchoNew(Echo::ShaderNodeUniform);
        m_uniformConfig->onVariableNameChanged.connectClassMethod(this, Echo::createMethodBind(&ColorDataModel::onVariableNameChanged));

        updateOutputDataVariableName();
    }

    QJsonObject ColorDataModel::save() const
    {
        QJsonObject modelJson = ShaderDataModel::save();

        ShaderUniformDataModel::saveUniformConfig(modelJson);

        modelJson["color"] = Echo::StringUtil::ToString(m_colorSelect->GetColor()).c_str();

        return modelJson;
    }

    void ColorDataModel::restore(QJsonObject const &p)
    {
        ShaderUniformDataModel::restoreUniformConfig(p);

        QJsonValue v = p["color"];
        if (!v.isUndefined())
        {
            Echo::String colorStr = v.toString().toStdString().c_str();
            m_colorSelect->SetColor(Echo::StringUtil::ParseColor(colorStr));
        }
    }

    void ColorDataModel::onColorEdited()
    {
        Q_EMIT dataUpdated(0);
        Q_EMIT dataUpdated(1);
    }

	void ColorDataModel::onSetAsParameter()
    {
        m_uniformConfig->setExport(true);

		updateOutputDataVariableName();

        onColorEdited();
	}

	void ColorDataModel::onSetAsConstant()
    {
        m_uniformConfig->setExport(false);
		updateOutputDataVariableName();

        onColorEdited();
	}

    void ColorDataModel::updateOutputDataVariableName()
    {
        Echo::String variableName = getVariableName();

		m_outputs[0] = std::make_shared<DataVector3>(this, "rgb");
		m_outputs[1] = std::make_shared<DataFloat>(this, "a");

		m_outputs[0]->setVariableName(Echo::StringUtil::Format("%s_Value.rgb", variableName.c_str()));
		m_outputs[1]->setVariableName(Echo::StringUtil::Format("%s_Value.a", variableName.c_str()));
    }

    void ColorDataModel::onVariableNameChanged()
    {
        updateOutputDataVariableName();

        onColorEdited();
    }

	bool ColorDataModel::generateCode(Echo::ShaderCompiler& compiler)
	{
		if (m_uniformConfig->isExport())
		{
			compiler.addUniform("vec4", getVariableName().c_str());

            compiler.addCode(Echo::StringUtil::Format("\tvec4 %s_Value = vec4(SRgbToLinear(fs_ubo.%s.rgb), fs_ubo.%s.a);\n", getVariableName().c_str(), getVariableName().c_str(), getVariableName().c_str()));
		}
		else
		{
			Echo::Color color = m_colorSelect->GetColor();
            color.toLinear();

			compiler.addCode(Echo::StringUtil::Format("\tvec4 %s_Value = vec4(%f, %f, %f, %f);\n", getVariableName().c_str(), color.r, color.g, color.b, color.a));
		}

		return true;
	}

    bool ColorDataModel::getDefaultValue(Echo::StringArray& uniformNames, Echo::VariantArray& uniformValues)
    {
        if (m_uniformConfig->isExport())
        {
            uniformNames.emplace_back("Uniforms." + getVariableName());
            uniformValues.emplace_back(m_colorSelect->GetColor());

            return true;
        }

        return false;
    }
}
