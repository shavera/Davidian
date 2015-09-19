using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace Davidian
{
    [KSPAddon(KSPAddon.Startup.MainMenu, false)]
    public class Davidian : MonoBehaviour
    {
        public void Awake() { }
        public void Start()
        {
            print("This is Davidian startup");
        }
        public void Update() { }
        public void FixedUpdate() { }
        public void OnDestroy() { }
    }
}
