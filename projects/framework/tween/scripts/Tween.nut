tween <- {}

// Globals
tween.manager <- null
tween.getManager <- function() {
    local m = tween.manager
    if(m == null) {
        m = TweenManager() // no need for "tween." because this == tween
        tween.manager = m
    }
    return m
}

/// \brief Singleton manager of all running tweens.
tween.TweenManager <- class
{
    curves = null
    _tweens = []

    constructor() {
        curves = {
            linear = function(x) { return x },
            inQuad = function(x) { return x*x},
            outQuad = function(x) { x = x*x; return 1 - x }
            // add some more?...
        }
    }

    /// \brief Updates all tweens for a duration of time.
    /// \param dt: duration of the frame
    function onUpdate(dt) {
        local tweens = _tweens
        for(local i = 0; i < tweens.len(); ++i) {
            local tween = tweens[i]
            tween.onUpdate(dt)
            if(tween.isFinished) {
                tweens.remove(i--)
            }
        }
    }


    function _addTween(tween) {
        _tweens.append(tween)
    }


    function _lerp(a, b, t) {
        return a + (b - a) * t
    }

}

/// \brief Instantiate this class to create a new tween.
class Tween
{
    isFinished = false

    _target = null
    _curve = null
    _t = 0
    _duration = 0
    _finishCallback = null
    _properties = []


    /// \brief creates a new tween.
    /// \param a_target: target instance or table (The tween will be able to animate some properties of it).
    /// \param a_duration: duration of the animation
    /// \param a_curve: name of the predefined curve of the animation
    constructor(a_target, a_duration=1.0, a_curve="linear") {
        local manager = ::tween.getManager()

        // Check arguments
        if(a_target == null) {
            println("Received null target!")
        }
        _target = a_target

        // Get curve
        local curves = manager.curves
        _curve = curves[a_curve] || curves.linear

        // Get duration
        if(a_duration < 0) {
            a_duration = 0
            isFinished = true
        }
        _duration = a_duration

        // Register to global update
        manager._addTween(this)
    }


    /// \brief Adds a property to animate.
    /// \param a_prop: name of the property or a setter method.
    /// \param a_beginValue
    /// \param a_endValue
    /// \return tween itself for chaining
    function addProperty(a_prop, a_beginValue, a_endValue) {
        if(a_prop == null) {
            println("Received null property!")
            return this
        }

        if(a_endValue == null)
            a_endValue = a_beginValue

        local p = {
            beginValue = a_beginValue,
            endValue = a_endValue,
        }

        local target = this._target
        local pt = typeof a_prop
        if(pt == "string") {
            // Bind property by name
            if(target[a_prop] == null) {
                println(a_prop + " doesn't exists on the given target!")
            }
            p.setter <- function(x) {
                target[a_prop] = x
            }
        }
        else if(pt == "function") {
            // Bind property by setter
            p.setter <- function(x) {
                return a_prop.call(target, x)
            }
        }
        else {
            println("Unecognized property argument type: " + pt)
        }

        _properties.append(p)
        return this
    }


    /// \brief Adds a function that will be called when the tween finishes
    /// \param cb: function with no required parameter
    /// \return tween itself for chaining
    function addFinish(cb) {
        _finishCallback = cb
        return this
    }


    /// \brief Updates the tween for an amount of delta time.
    /// This is automatically called by TweenManager.
    /// \param dt: frame duration
    function onUpdate(dt) {
        _t += dt
        if(_t < _duration) {
            // Animation is playing
            local k = _t / _duration
            local curve = _curve
            local lerp = tween.getManager()._lerp
            foreach(p in _properties) {
                local x = lerp(p.beginValue, p.endValue, curve(k))
                p.setter(x)
            }
        }
        else {
            // Animation just finished
            if(!isFinished) {
                foreach(p in _properties) {
                    p.setter(p.endValue)
                }
                if(_finishCallback)
                    _finishCallback()
                isFinished = true
            }
        }
    }

}
