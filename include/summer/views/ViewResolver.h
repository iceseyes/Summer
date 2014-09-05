/*
 * Summer - /Summer/include/summer/views/ViewResolver.h
 * author: Massimo Bianchi 2014
 */

#ifndef VIEWRESOLVER_H_
#define VIEWRESOLVER_H_

#include <summer/views/DirectFileView.h>

#include <string>

namespace summer { namespace views {

template<typename Reply> class ViewResolver {
public:
	std::string appContext;

	template<class View> void operator()(Reply &reply, const View &view) {
		(reply << view.headers()).content = view.body();
	}

	void operator()(Reply &reply, const std::string &view) {
		operator()(reply, DirectFileView(appContext + "/web/" + view));
	}
};

}}

#endif /* VIEWRESOLVER_H_ */
