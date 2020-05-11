#pragma once

#include "core/scene/IProxyMesh.hpp"

namespace sibr {
	/**
	\ingroup sibr_scene
	*/
	class SIBR_SCENE_EXPORT ProxyMesh : public IProxyMesh {
		SIBR_DISALLOW_COPY(ProxyMesh);
	public:
		typedef std::shared_ptr<ProxyMesh>					Ptr;

		ProxyMesh::ProxyMesh() {};
		~ProxyMesh() {};
		void												loadFromData(const IParseData::Ptr & data) override;
		void												replaceProxy(Mesh::Ptr newProxy) override;
		void												replaceProxyPtr(Mesh::Ptr newProxy) override;
		bool												hasProxy(void) const;
		const Mesh&											proxy(void) const;
		const Mesh::Ptr										proxyPtr(void) const;

	protected:

		Mesh::Ptr											_proxy;

	};

	inline bool												sibr::ProxyMesh::hasProxy(void) const
	{
		return _proxy && !_proxy->vertices().empty();
	}

	inline const Mesh& ProxyMesh::proxy(void) const
	{
		return *_proxy;
	}

	inline const Mesh::Ptr									ProxyMesh::proxyPtr(void) const
	{
		return _proxy;
	}

};